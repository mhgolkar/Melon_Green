/*
 * Melon Green v1.1.0
 * Morteza H. Golkar
 * 2017
 * -----------------------------
 * EEPROM Memory Addresses
 * -----------------------------
*/

// First Run?
	#define EEPROMCHK 0
// Calibration
    #define EELdrMin  1
    #define EELdrMax  2
    #define EESmtMin  3
    #define EESmtMax  4
    #define EEFscMin  5
    #define EEFscMax  6
    //#define EEFscRain .. //-> old version
// Settings
    // Time, Date & Regional
        #define EEDayLightTresh     7
        #define EENightLightTresh   8
        /*
        #define EELatitude   ..
        #define EELongitude  ..
        #define EEUTCMinutes ..
        #define EEQuadrant   ..
        // --> Reserved for Further Use
        */
        #define EEYear      9
        #define EEMonth     10
        #define EEDay       11
        #define EEHour      12
        #define EEMinute    13
        #define EESecond    14
    // Irrigation
        #define EEIrigTrigSmt 15 // Triggers
        #define EEIrigTrigInt 16
        #define EEIrigIntUnit 17
        #define EEIrigTrigTmp 18
        #define EEIrigTrigHum 19
        #define EEIrigLimDay  20 // Limiters
        #define EEIrigLimRin  21
        #define EEIrigLimSmt  22
        #define EEIrigLimTim  23
        #define EEIrigMaxpDay 24
    // Grow Light Supplier
        #define EEGrowTrigInt           25
        #define EEGrowIntUnit           26
        #define EEGrowEachDuration      27
        #define EEGrowTrigMin           28
        #define EEGrowTrigMax           29
        #define EEGrowLightSupplyBefore 30
    // Temperature & Humidity Control
        #define EETempHumCTemp      31
        #define EETempHumCTempNight 32
        #define EETempHumCTTol      33
        #define EETempHumCHum       34
        #define EETempHumCHumNight  35
        #define EETempHumCHTol      36
        #define EEFANCforDeHumid    37
    // Feeding Solution Mixer
        #define EEFeedFrtlzRate 38
        #define EEFeedFrtlzFlow 39
        #define EEFeedContainer 40
// Action Memory
    // Last Irrigation
        #define EENextIrigYear      41
        #define EENextIrigMonth     42
        #define EENextIrigDay       43
        #define EENextIrigHour      44
        #define EENextIrigMinute    45
    // Last Grow Light
        #define EENextGrowYear      46
        #define EENextGrowMonth     47
        #define EENextGrowDay       48
        #define EENextGrowHour      49
        #define EENextGrowMinute    50
    // Duty Cycle
        #define EEDutyCycle         51
        #define EEDutyCycleUnit     52
        #define EEDutyCycleWarmUp   53
