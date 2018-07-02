/*
 * Melon Green v1.1.0
 * Morteza H. Golkar
 * 2017
 * -----------------------------
 * Presets
 * CAUTION! It's Configured for v1.x.x Boards Pinout
 * -----------------------------
*/

#define BAUDRATE 9600 // SERIAL BAUD RATE
// Pinout
#define SVCC 2 // TOGGLING POWER
// Sensors, Modules etc
#define DEBM A0 // Debug Mode
#define SOILMOISTURE A1 // ANALOG SOIL MOISTURE SENSOR
#define LDR A2 // LIGHT DEPENDENT RESISTOR (PHOTOCELL / ANALOG LIGHT SENSOR)
#define FOODTANK A3 // SOLUTION MIXER CONTAINER
#define DHTPIN 3 // Humidity & Temprature
#define RAIN 4 // Rain
// Relays
#define IRIG 5 // IRRIGATION
#define GROW 6 // GROW LIGHT SUPPLIER
#define FANC 7 // FAN, COOLER
#define HEAT 8 // HEATER
#define FOGG 9 // FOGGER
#define FRTL 11 // FERTILIZER (SOLUTION MIXER)
#define WATR 12 // WATER (SOLUTION MIXER)
// Delays & Timing
#define DHTDELAY 1500 // Reading temperature or humidity takes about 250 milliseconds! Sensor readings may also be up to 2 seconds (very slow 'old' sensor)
#define ANALOGDELAY 1 // It takes about 100 microseconds (0.0001 s) to read an analog input
#define ADJUSTINT 60000 // (should be = x*60,000) automatic adjusting internalclock after x miliseconds of delays
#define DENOISEA 3 // x attempts (integer)...
#define DENOISES 1 // ... in y seconds (integer) for Noise Reduction

// Calibration [Preset]
#define calibLDRMin 0
#define calibLDRMax 255
#define calibSmtMin 0
#define calibSmtMax 255
// #define calibTolerance 10 // -> Old Version 1.0.0.
#define calibFdTMin 100
#define calibFdTMax 170
//#define calibFdTRain 237 //-> old version

// More
#define UNIX_DAY_ZERO 1970
