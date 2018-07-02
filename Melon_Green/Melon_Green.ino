/*
 * Melon Green v1.2.0
 * Morteza H. Golkar
 * [2018]
*/

// Info
// [Arduino 1.8.0+ ]
#define Welcome_Message "Welcome. It's..."
#define Melon_Green_Info "Melon Green v1.2.0 \r\n[M. H. Golkar 2018]"

// Modes [User Editable]
#define _RTC // I2C Real Time Clock Module (e.g. DS323x) is Available for Accurate timing
#define _DHT // DHT Module is Available
#define _Mixer // Mix Feeding Solution Using Two Pumps and a Container
#define _Serial // Serial Communication With Board (RECOMMENDED)
#define _SynthCycle // Synthetic Day/Night Cycle

// Presets & Settings
#include "modules/Settings.h"
#include "modules/Presets.v1.h"
#include "modules/Memory_Addresses.h"
// Required Libraries & Structures
	// Volatile Memory
	#include <EEPROM.h>
	// Digital Humidity & Tempreture Sensor (RECOMMENDED)
	#ifdef _DHT
		// Adafruit_DHT...
		#include <Adafruit_Sensor.h>
		#include <DHT.h>
		#include <DHT_U.h>
		DHT dht(DHTPIN, DHTTYPE);
	#endif
	// Time
	#include <Time.h>
	#include <TimeLib.h>
	boolean INTERNAL_CLOCK = false;
	#ifdef _RTC
		// Real Time Clock & Calendar (Optional, Recommended) is Available
		#include <Wire.h> // I2C Bus -> COUTION! M.G. Freezes if module damages etc
		#include <RTClib.h> // Adafruit_RTClib
		RTCMODULE rtc;
	#endif

// Variables
int Now[6];	unsigned long int forgotten = 0; // Timepiece
unsigned int lux; unsigned int moist; unsigned int food; float temp; float humid;	// General Variables
boolean awake = false; boolean warm = false; unsigned long warmUpTime; unsigned long lastDuty = 0;	// Duty Cycle
boolean diag = false; unsigned long diagTime;	// Diagnostics
boolean is_irrigating = false; unsigned long IrrigationTime; int Total_Irrig_per_Day = 0; int Today = 0; // Irrigation
boolean is_supplyingGrowLight = false; unsigned long growLightTime; // Grow Light Supplier
boolean is_heating = false; boolean is_cooling = false;	// temperature keeper
boolean is_Fanning = false; boolean is_Fogging = false;	// humidity keeper
boolean is_Mixing = false; boolean fertilizer_added = false; unsigned long fertilizerTime;	// Mixer
// Analog Sensors Noise Reduction
int ldr_status = 0; unsigned long int ldr_first_attempt = 0; unsigned long int ldr_bank = 0;
int smt_status = 0; unsigned long int smt_first_attempt = 0; unsigned long int smt_bank = 0;
int fdt_status = 0; unsigned long int fdt_first_attempt = 0; unsigned long int fdt_bank = 0;
// Serial [Event] Command Line Interface
boolean debugMode = false;
String inputString = "0";	// a string to hold incoming data
boolean stringComplete = true;	// whether the string is complete
int command = 0; int cliStep = 0; bool firstrun = true;
// Setting Parameters -> True = NO ACTION
boolean parametering = false;

// More Code Blocks
#include "modules/Essential_Functions.h"
#ifdef _Serial
  #include "modules/Serial_Interface.h"
#endif

void setup() {
 // the setup function runs once when you press reset or power the board
 #ifdef _Serial
  setupSerial();
 #endif
// Initialize (Essential_Functions.h)
    // Writing Defult Presets to EEPROM
    initializePresets(); // Essential_Functions.h
    // Seting Pin Mode...
    setupPinout();
    // Sensors & Modules
    initializeModules(); // (DHT, RTC/Time)
	inputString.reserve(3); // Resrve 3 Chars for CLI
 #ifdef _Serial
  endSerial();
 #endif
 updateNow();
 Today = Now[2];
}

void loop() {
  // the loop function runs over and over while board is on
  #ifdef _Serial
    // Serial_interface.h
    toggleDebugMode();
    // Command Line Serial Interface
    if( debugMode == true ) { serialCLI(); }
  #endif
  if( !parametering ){
    // Essential_Functions.h
    dutyCycle(); // awake? warm up?
    if( diag == true ) diagTimekeeper(); // Manual Diagnostics Time Keeper
    if( is_irrigating == true ) { limitIrrigation(); } else if( is_Mixing == false && awake == true ){ checkIrrigationJob(); }
    if( is_supplyingGrowLight == true ) { limitGrowLight(); } else if( awake == true ) { checkGrowLightJob(); }
    #ifdef _DHT
      temperatureKeeper(awake);
      humidityKeeper(awake);
    #endif
    #ifdef _Mixer
      solutionMixer(awake);
    #endif
  }
}
