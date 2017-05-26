/*
 * Melon Green v1.0.0
 * Morteza H. Golkar
 * 2017
 * -----------------------------
 * Settings
 * -----------------------------
*/

/* Note:
 * All Settings Are Accessible from Serial CLI
 * ( Except: <Sensors>, DiagMaxMilliSecond & century )
*/
 
// Sensors
	#define RTCMODULE RTC_DS3231 // Real Time Clock Module Type: RTC_DS3231, RTC_DS1307, RTC_PCF8523, RTC_PCF8563
	#define DHTTYPE DHT11 // Humidity & Temperature Module: DHT22 (AM2302), DHT21, DHT11 (AM2301)
// Duty Cycle
	#define DutyCycle 0 // x Cycles... [0 = Sleepless]
	#define DutyCycleUnit 1 // ... per [0 = Second, 1 = Minute, 2= Hour, 3 = Day]
	#define DutyCycleWarmUp 2 // (Seconds) time for sensors to warm up; if it's necessery.
	#define DiagMaxMilliSecond 30000 // (milliseconds) Duration of Manual Diagnostics -> Auto Off
// Time, Date & Regional //
	#define century 2000
	/*
	// --> Reserved for Further Use
	#define latitude 00.0000 //° N
	#define longitude 000.0000 //° E
	#define time_zone 00000 // UTC Offset in minutes
	#define quadrant 0 // Wich Quadrant of the World? [0] NW [1] NE [2] SW [3] SE
	*/
	// Day & Sunset
	#define DayLightThreshold 30 //% Brighter than x% -> Detects Day
	#define NightLightThreshold 10 //% Darker than y% -> Detects Night
// Irrigation
	#define IrrigateOnSoilMoistureBelow 100 //% [100 = OFF]
	#define IrrigationOnIntervalsEvery 0 //[0 = OFF]
	#define IrrigateIntervalUnit 1 // [0 = Minutes, 1= Hours, 2 = Days, 3 = Week, 4 = Month]
	#define IrrigationOnTemperatureHigherThan 0 //'C [0->OFF]
	#define IrrigationOnHumidityBelow 100 //% Relative [100->OFF]
	#define IrrigateAtDaytimeOnly  1 // [0 = OFF]
	#define DontIrrigateOnRainyDays 1 // [0 = OFF]
	#define IrrigateToMaximumSoilMoisture 40 // % Maximum Allowable Soil Moisture
	#define IrrigationDurationEachTime  2 // minutes
// Grow Light Supplier
	#define GrowLightIntervalsEvery 0 //[0 = OFF]
	#define GrowLightIntervalUnit 1 // [0 = Minutes, 1= Hours, 2 = Days, 3 = Week, 4 = Month]
	#define GrowLightSupplementDurationEachTask 0
	#define GrowLightOnIfBelow 100
	#define GrowLightOFFIfAbove 0
	#define DontSupplyGrowLightAfter 24 // 0-23 O'Clock [> 23 -> OFF]
// Temperature & Humidity
	#define KeepTemperatureAbout 0 //[0 = OFF]
	#define TemperatureTolerance 5 //'C
	#define keepHumidityAbout 0 //[0 = OFF]
	#define HumidityTolerance 10 //%
	#define UseFanCoolerToReduceHumidity 0 // [0] NO [1] YES
// Feeding Solution Mixer
	#define FertilizerRate 0 //% of Mixture
	#define FertilizerPumpFlowRate 0 // Liter/Second
	#define SolutionContainerVolume 0 // Liter
