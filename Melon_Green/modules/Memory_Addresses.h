/*
 * Melon Green v1.0.0
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
    // Grow Light Supplier
        #define EEGrowTrigInt           24
        #define EEGrowIntUnit           25
        #define EEGrowEachDuration      26
        #define EEGrowTrigMin           27
        #define EEGrowTrigMax           28
        #define EEGrowLightSupplyBefore 29
    // Temperature & Humidity Control
        #define EETempHumCTemp      30
        #define EETempHumCTTol      31
        #define EETempHumCHum       32
        #define EETempHumCHTol      33
        #define EEFANCforDeHumid    34
    // Feeding Solution Mixer
        #define EEFeedFrtlzRate 35
        #define EEFeedFrtlzFlow 36
        #define EEFeedContainer 37
// Action Memory
    // Last Irrigation
        #define EENextIrigYear      38
        #define EENextIrigMonth     39
        #define EENextIrigDay       40
        #define EENextIrigHour      41
        #define EENextIrigMinute    42
    // Last Grow Light
        #define EENextGrowYear      43
        #define EENextGrowMonth     44
        #define EENextGrowDay       45
        #define EENextGrowHour      46
        #define EENextGrowMinute    47
    // Duty Cycle
        #define EEDutyCycle         48
        #define EEDutyCycleUnit     49
        #define EEDutyCycleWarmUp   50


