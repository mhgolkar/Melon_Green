/*
 * Melon Green v1.2.0
 * Morteza H. Golkar
 * 2018
 * ------------------------------------------
 * Serial Command Line Interface (Debug Mode)
 * ------------------------------------------
*/

void SerialPageBreak(){
    //Serial.write(12);
    Serial.write(27);
    Serial.print("[2J");
    Serial.write(27);
    Serial.print("[H");
}
void setupSerial(){
    Serial.begin(BAUDRATE);
	//while( Serial.available() < 1 ){};
    SerialPageBreak(); // Page Break in [Standard] Terminal (Doesn't work for Arduino IDE Serial Monitor)
    Serial.println( F( Welcome_Message ) ); Serial.println( F( Melon_Green_Info )  );
}
void endSerial(){
    	//Serial.flush();
		Serial.end();
}
// Debug Mode
void toggleDebugMode(){
	if( debugMode == false && digitalRead(DEBM) == HIGH ){
		#ifdef _Serial
		  setupSerial();
		#endif
		debugMode = true;
	} else if( debugMode == true && digitalRead(DEBM) == LOW ) {
        debugMode = false;
        #ifdef _Serial
            endSerial();
        #endif
    }
}
// Serial Command Line Interface
void serialCLI(){
    // Serial Event
    if (stringComplete) {
        if(cliStep == 0) { command = inputString.toInt(); }
        // Menus
        if( command == 0 ){ // Prints Main Menu
            if(cliStep == 0) { if(firstrun != true){ SerialPageBreak(); } else { firstrun = false; } } else { cliStep = 0; }
            Serial.println( F("MAIN MENU\r\n---------\r\n[1] Reading Sensors \r\n[2] Calibration \r\n[3] Manual Diagnostics\r\n[4] Settings >>") );
            Serial.println( F("-------------------\r\n* All Commands are Unique\r\n[0]  Main Menu") );
            if(parametering == false) { Serial.println( F("[99] Disable All Functions") ); } else { Serial.println( F("[99] Enable All Functions") ); }
        }
        if( command == 1 ){ SerialPageBreak(); Serial.println( F("[1] READING SENSORS\r\n-------------------\r\n[5]  Time & Date\r\n[6]  Humidity & Temperature\r\n[7]  Day Light Intensity\r\n[8]  Soil Moisture \r\n[9]  Food Tank\r\n[10] Rain Detection >>") ); } // Prints Sensor Reading Menu
        if( command == 2 ){ SerialPageBreak(); Serial.println( F("[2] CALIBRATION\r\n---------------\r\nDay Light Sensor (LDR) [11] Min [12] Max \r\nSoil Moisture [13] Min [14] Max \r\nFood Tank [15] Min [16] Max >>") ); } // Prints Calibration Menu
        if( command == 3 ){ SerialPageBreak(); Serial.println( F("[3] MANUAL DIAGNOSTICS\r\n----------------------\r\n[17] Irrigation\r\n[18] Grow Light Supply\r\n[19] Fan / Cooler\r\n[20] Heater\r\n[21] Humidifier\r\n-----------\r\n* Use for Diagnosis Only (Auto Off)\r\n[22] Stop All >>") ); } // Prints Manual Diagnostics Menu
        if( command == 4 ){ // Settings Menu
            SerialPageBreak();
            Serial.print( F("[4] SETTINGS\r\n------------\r\n[23] Time, Date & Regional\r\n[24] Irrigation\r\n[25] Grow Light Supplier\r\n[26] Temperature & Humidity Control") );
            #ifdef _Mixer
                Serial.println( F("\r\n[27] Feeding Solution >>") );
            #else
                Serial.println( F(" >>") );
            #endif
        }
        if( command == 23 ){ // time, date & regional settings menu
            SerialPageBreak();
            Serial.println( F("[23] TIME, DATE & REGIONAL\r\n-----------------------") );
			/*
            Serial.print( F("[..] Coordinates: ") ); Serial.print( EEPROM.read(EELatitude) ); if( EEPROM.read(EEQuadrant) < 2 ){ Serial.print("'N "); } else{ Serial.print("'S "); } Serial.print( EEPROM.read(EELongitude) ); if( (EEPROM.read(EEQuadrant)%2) == 0 ){ Serial.println("'W "); } else{ Serial.println("'E "); }
            Serial.print( F("[..] UTC (Minutes): ") ); if( (EEPROM.read(EEQuadrant)%2) == 0 ){ Serial.print("-"); } else{ Serial.print("+"); } Serial.println( EEPROM.read(EEUTCMinutes) );
			// --> Reserved for Further Use
			*/
			Serial.print( F("[28] Duty Cycle: ") ); if( (int)EEPROM.read(EEDutyCycle) == 0 ) { Serial.println("Sleepless"); } else { Serial.print( EEPROM.read(EEDutyCycle) ); Serial.print( F(" Cycles/") ); if( EEPROM.read(EEDutyCycleUnit) == 0) { Serial.print("Second"); } else if( EEPROM.read(EEDutyCycleUnit) == 2) { Serial.print("Hour"); } else if( EEPROM.read(EEDutyCycleUnit) == 3) { Serial.print("Day"); } else { EEPROM.update(EEDutyCycleUnit, 1); Serial.print("Minute"); } Serial.print(" (+"); Serial.print( EEPROM.read(EEDutyCycleWarmUp) ); Serial.println(" Sec WarmUp)");}
			Serial.print( F("[29] Day/Night Brightness Threshold: ") ); Serial.print( EEPROM.read(EEDayLightTresh) ); Serial.print("% / "); Serial.print( EEPROM.read(EENightLightTresh) ); Serial.println("%");
            Serial.print( F("[30] Date & Time: ") ); printDateTime();
        }
        if( command == 24 ){ // Irrigation Settings Menu
            SerialPageBreak();
            Serial.println( F("[24] IRRIGATION\r\n---------------") );
            Serial.print( F("[31] On Soil Moisture Below: ") ); if( (int)EEPROM.read(EEIrigTrigSmt) >= 100 ) { Serial.println("OFF"); } else { Serial.print( EEPROM.read(EEIrigTrigSmt) ); Serial.println(" %");}
            Serial.print( F("[32] Scheduled Task; Every: ") ); if( (int)EEPROM.read(EEIrigTrigInt) == 0 ) { Serial.println("OFF"); } else { Serial.print( EEPROM.read(EEIrigTrigInt) ); if( EEPROM.read(EEIrigIntUnit) == 0) { Serial.println(" Minutes"); } else if( EEPROM.read(EEIrigIntUnit) == 2) { Serial.println(" Days"); } else if( EEPROM.read(EEIrigIntUnit) == 3) { Serial.println(" Weeks"); } else if( EEPROM.read(EEIrigIntUnit) == 4) { Serial.println(" Months"); } else { EEPROM.update(EEIrigIntUnit, 1); Serial.println(" Hours"); } }
            Serial.print( F("[33] Next Irrigation: ") ); if ( (int)EEPROM.read(EEIrigTrigInt) == 0 ) { Serial.println("OFF"); } else { printNextIrrigation(); }
            #ifdef _DHT
                Serial.print( F("[34] On Temperature Higher Than: ") ); if( (int)EEPROM.read(EEIrigTrigTmp) <= 0 ) { Serial.println("OFF"); } else { Serial.print( EEPROM.read(EEIrigTrigTmp) ); Serial.println(" 'C");}
                Serial.print( F("[35] On Humidity Below: ") ); if( (int)EEPROM.read(EEIrigTrigHum) >= 100 ) { Serial.println("OFF"); } else { Serial.print( EEPROM.read(EEIrigTrigHum) ); Serial.println(" %");}
            #endif
			Serial.print( F("[36] Irrigate at Daytime Only: ") ); if( (int)EEPROM.read(EEIrigLimDay) == 0 ) { Serial.println("OFF"); } else { Serial.println("ON");}
            Serial.print( F("[37] Don't Irrigate if It's Rainy: ") ); if( (int)EEPROM.read(EEIrigLimRin) == 0 ) { Serial.println("OFF"); } else { Serial.println("ON");}
            Serial.print( F("[38] Maximum Allowable Soil Moisture: ") ); if( (int)EEPROM.read(EEIrigLimSmt) == 0 ) { Serial.println("OFF"); } else { Serial.print( EEPROM.read(EEIrigLimSmt) ); Serial.println(" %"); }
            Serial.print( F("[39] Duration of Each Irrigation: ") ); if( (int)EEPROM.read(EEIrigLimTim) == 0 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEIrigLimTim) ); Serial.println(" Minutes"); }
            Serial.print( F("[51] Maximum Irrigation per Day: ") ); if( (int)EEPROM.read(EEIrigMaxpDay) == 0 ) { Serial.println("NULL [Dangerous]"); } else { Serial.print( EEPROM.read(EEIrigMaxpDay) ); Serial.print(" Minutes [- "); Serial.print(Total_Irrig_per_Day); Serial.println("]"); }
			Serial.println( F("----------------------------\r\nNote: [51] keeps minutes after each irrigation in short-term memory.\r\n[38] & [39] are Recommended.") );
        }
        if( command == 25 ){// Grow Light Supply Settings Menu
            SerialPageBreak();
            Serial.println( F("[25] GROW LIGHT SUPPLIER\r\n------------------------") );
            Serial.print( F("[40] Scheduled Task; Every: ") ); if( (int)EEPROM.read(EEGrowTrigInt) == 0 ) { Serial.println("OFF"); } else { Serial.print( EEPROM.read(EEGrowTrigInt) ); if( EEPROM.read(EEGrowIntUnit) == 0) { Serial.println(" Minutes"); } else if( EEPROM.read(EEGrowIntUnit) == 2) { Serial.println(" Days"); } else if( EEPROM.read(EEGrowIntUnit) == 3) { Serial.println(" Weeks"); } else if( EEPROM.read(EEGrowIntUnit) == 4) { Serial.println(" Months"); } else { EEPROM.update(EEGrowIntUnit, 1); Serial.println(" Hours"); } }
			Serial.print( F("[41] Next Supplement: ") ); if ( (int)EEPROM.read(EEGrowTrigInt) == 0 ) { Serial.println("OFF"); } else { printNextGrowLight(); }
            Serial.print( F("[42] Each Task Duration: ") ); if( (int)EEPROM.read(EEGrowEachDuration) == 0 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEGrowEachDuration) ); Serial.println(" Minutes"); }
            Serial.print( F("[43] Turn G.L. ON if It's Darker Than: ") ); if( (int)EEPROM.read(EEGrowTrigMin) >= 100 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEGrowTrigMin) ); Serial.println(" %");}
            Serial.print( F("[44] Turn G.L. OFF if It's Brighter Than: ") ); if( (int)EEPROM.read(EEGrowTrigMax) == 0 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEGrowTrigMax) ); Serial.println(" %");}
			Serial.print( F("[45] Supply Grow Light during Day & Before: ") ); if( (int)EEPROM.read(EEGrowLightSupplyBefore) > 23 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEGrowLightSupplyBefore) ); Serial.println( F(" O'clock") );}
			Serial.println( F("-----------------------------------\r\nNotice: Keep LDR Away From Grow Light Supplier Sources.") );
        }
        if( command == 26 ){ // temperature & humidity control Settings Menu
            SerialPageBreak();
			#ifdef _DHT
                Serial.println( F("[26] TEMPERATURE & HUMIDITY CONTROL\r\n-----------------------------------") );
                Serial.print( F("[46] Keep Temperature About:\r\n\t ") ); if( (int)EEPROM.read(EETempHumCTemp) == 0 ) { Serial.print("OFF"); } else { Serial.print( EEPROM.read(EETempHumCTemp) ); } Serial.print(" Day | "); if( (int)EEPROM.read(EETempHumCTempNight) == 0 ) { Serial.print("OFF"); } else { Serial.print( EEPROM.read(EETempHumCTempNight) ); } Serial.print(" Night {+/- "); Serial.print( EEPROM.read(EETempHumCTTol) ); Serial.println("} 'C");
                Serial.print( F("[47] Keep Humidity About:\r\n\t ") ); if( (int)EEPROM.read(EETempHumCHum) == 0 ) { Serial.print("OFF"); } else { Serial.print( EEPROM.read(EETempHumCHum) ); } Serial.print(" Day | "); if( (int)EEPROM.read(EETempHumCHumNight) == 0 ) { Serial.print("OFF"); } else { Serial.print( EEPROM.read(EETempHumCHumNight) ); } Serial.print(" Night {+/- "); Serial.print( EEPROM.read(EETempHumCHTol) ); Serial.print("} %"); if( (int)EEPROM.read(EEFANCforDeHumid) == 1 ){ Serial.println( F(" +FAN+") ); } else { Serial.println(); }
                Serial.println( F("-------------------------------\r\n* It Works With Humidifier, Cooler & Heater Functions.") );
			#else
				Serial.println( F("UNAVAILABLE (COMPILED WITHOUT DHT)") );
			#endif
        }
        #ifdef _Mixer
            if( command == 27 ){ // Feeding Solution Settings Menu
                SerialPageBreak();
                Serial.println( F("[27] FEEDING SOLUTION\r\n---------------------") );
                Serial.print( F("[48] Fertilizer Rate: ") ); if( (int)EEPROM.read(EEFeedFrtlzRate) == 0 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEFeedFrtlzRate) ); Serial.println(" % of Solution"); }
                Serial.print( F("[49] Fertilizer Pump Flow Rate: ") ); if( (int)EEPROM.read(EEFeedFrtlzFlow) == 0 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEFeedFrtlzFlow) ); Serial.println(" Liter/Sec"); }
                Serial.print( F("[50] Solution Container volume: ") ); if( (int)EEPROM.read(EEFeedContainer) == 0 ) { Serial.println("NULL"); } else { Serial.print( EEPROM.read(EEFeedContainer) ); Serial.println(" Liter"); }
                Serial.println( F("----------------\r\n* All Fields Are Required\r\nMaximum Allowable Value for All of Parameters is 255 (1 Byte Integer).\r\nUse Proportionality (Math) for Greater Values.") );
            }
        #endif
        // Sub-Menus
        /* // --> Reserved for Further Use
		if( command == 28 ){ // Coordinates
            if(cliStep == 0){
                Serial.println( F("Which quadrant of the world you are in?\r\n[0]NW [1]NE [2]SW [3]SE") );
                cliStep = 1;
            } else if(cliStep == 1){
                EEPROM.update(EEQuadrant, inputString.toInt() );
                Serial.println( F("What's Your Farm Latitude?") );
                cliStep = 2;
            } else if (cliStep == 2) {
                Serial.println( F("& Longitude?") );
                EEPROM.update(EELatitude, inputString.toInt() );
                cliStep = 3;
            } else if (cliStep == 3) {
                EEPROM.update(EELongitude, inputString.toInt() );
                cliStep = 0;
                inputString = "23";
                return;
            }
        }
        if( command == 29 ){ // UTC
            if(cliStep == 0){
                Serial.println( F("What's Your Farm's UTC Offset? (Minutes, Posetive)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEUTCMinutes, inputString.toInt() );
                cliStep = 0;
                inputString = "23";
                return;
            }
        }
		*/
        if( command == 28 ){ // Duty Cycle
            if(cliStep == 0){
                //Serial.println( F("Duty Cycle Settings Doesn't Impact on Regular Jobs (e.g. Irrigation Time, Scheduled Events). It's all about sensors.") );
                Serial.println( F("How Many Cycles? (Max 255, 0 -> Sleepless)") );
                cliStep = 1;
            } else if (cliStep == 1) {
				int temp = inputString.toInt();
                EEPROM.update(EEDutyCycle, temp);
				if(temp == 0){ // Sleepless
					cliStep = 0;
					inputString = "23";
					return;
				} else {
					Serial.println( F("... per? [0] Second [1] Minute  [2] Hours [3] Day") );
					cliStep = 2;
				}
            } else if (cliStep == 2) {
                Serial.println( F("How Long Does It Take to Sensors to Warm up? (Max 255, Seconds)") );
                EEPROM.update(EEDutyCycleUnit, inputString.toInt() );
                cliStep = 3;
            } else if (cliStep == 3) {
                EEPROM.update(EEDutyCycleWarmUp, inputString.toInt() );
                cliStep = 0;
                inputString = "23";
                return;
            }
        }
		if( command == 29 ){ // Day/Night Light Threshold
            if(cliStep == 0){
                Serial.println( F("How Much is Day's Brightness Treshold?") );
                cliStep = 1;
            } else if(cliStep == 1){
                EEPROM.update(EEDayLightTresh, inputString.toInt() );
                Serial.println( F("... & Night's Treshold?") );
                cliStep = 2;
            } else if (cliStep == 2) {
                EEPROM.update(EENightLightTresh, inputString.toInt() );
                cliStep = 0;
                inputString = "23";
                return;
            }
		}
        if( command == 30 ){ // Date & Time Adjustment
            if(cliStep == 0){
                Serial.println( F("What Year is it? (Two Digits: 2018 -> 18)") );
                cliStep = 1;
            } else if(cliStep == 1){
                EEPROM.update(EEYear, inputString.toInt());
                Serial.println( F("Month? (1 - 12)") );
                cliStep = 2;
            } else if (cliStep == 2) {
                Serial.println( F("Day? (1 - 31)") );
                EEPROM.update(EEMonth, inputString.toInt());
                cliStep = 3;
            } else if (cliStep == 3) {
                Serial.println( F("Hour? (0 - 23)") );
                EEPROM.update(EEDay, inputString.toInt());
                cliStep = 4;
            } else if (cliStep == 4) {
                Serial.println( F("Minute? (0 - 59)") );
                EEPROM.update(EEHour, inputString.toInt());
                cliStep = 5;
            } else if (cliStep == 5) {
                Serial.println( F("Second? (0 - 59)") );
                EEPROM.update(EEMinute, inputString.toInt());
                cliStep = 6;
            } else if (cliStep == 6) {
                EEPROM.update(EESecond, inputString.toInt());
                //adjust:
                /*
                // (1.2.0 r0)
                #ifdef _RTC 
                    rtc.adjust(DateTime((int)((int)EEPROM.read(EEYear) + century), (int)EEPROM.read(EEMonth), (int)EEPROM.read(EEDay), (int)EEPROM.read(EEHour), (int)EEPROM.read(EEMinute), (int)EEPROM.read(EESecond)));
                #endif
                */
                setTime((int)EEPROM.read(EEHour), (int)EEPROM.read(EEMinute), (int)EEPROM.read(EESecond), (int)EEPROM.read(EEDay), (int)EEPROM.read(EEMonth), (int)EEPROM.read(EEYear));
                // 1.2.0 r2
                #ifdef _RTC
                    rtc.adjust(now());
                #endif  
                cliStep = 0;
                inputString = "23";
                return;
            }
        }
        if( command == 31 ){ // Irrigate On Soil Moisture Below...
            if(cliStep == 0){
                Serial.println( F("How Much Soil Moisture Would Trigger Irrigation? (0-99%, 100 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigTrigSmt, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
        if( command == 32 ){ // Irrigation On Intervals
            if(cliStep == 0){
                Serial.println( F("Delay Between Each Irrigation: (Max 255, 0 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                Serial.println( F("[0] Minutes [1] Hours [2] Days [3] Weeks [4] Months ?") );
                EEPROM.update(EEIrigTrigInt, inputString.toInt() );
                cliStep = 2;
            } else if (cliStep == 2) {
                EEPROM.update(EEIrigIntUnit, inputString.toInt() );
                cliStep = 0;
                inputString = "33";
				        if( EEPROM.read(EENextIrigMonth) > 12 || EEPROM.read(EENextIrigMonth) < 1 ){ inputString = "33"; } else { inputString = "24"; };
                return;
            }
        }
        if( command == 33 ){ // Next Run?
            if( (int)EEPROM.read(EEIrigTrigInt) == 0 ) { // Bounded to [32]
                Serial.println("Disabled! Take a look at [32] Scheduled Task...");
                cliStep == 0;
            } else {
                if(cliStep == 0){
                    Serial.println( F("Next Task: Year? (Two Digits: 2018 -> 18)") );
                    cliStep = 1;
                } else if(cliStep == 1){
                    EEPROM.update(EENextIrigYear, inputString.toInt() );
                    Serial.println( F("Month? (1 - 12)") );
                    cliStep = 2;
                } else if (cliStep == 2) {
                    Serial.println( F("Day? (1 - 31)") );
					EEPROM.update(EENextIrigMonth, inputString.toInt() );
                    cliStep = 3;
                } else if (cliStep == 3) {
                    Serial.println( F("Hour? (0 - 23)") );
					EEPROM.update(EENextIrigDay, inputString.toInt() );
                    cliStep = 4;
                } else if (cliStep == 4) {
                    Serial.println( F("Minute? (0 - 59)") );
					EEPROM.update(EENextIrigHour, inputString.toInt() );
                    cliStep = 5;
                } else if (cliStep == 5) {
					EEPROM.update(EENextIrigMinute, inputString.toInt() );
                    cliStep = 0;
                    if( EEPROM.read(EEIrigLimTim) == 0 ){ inputString = "39"; } else { inputString = "24"; };
                    return;
                }
            }
        }
		#ifdef _DHT
        if( command == 34 ){ // Irrigation On Temperature Higher Than
            if(cliStep == 0){
                Serial.println( F("Irrigate if It's Hotter Than: ('C, 0 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigTrigTmp, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
        if( command == 35 ){ // Irrigation On Humidity Below
            if(cliStep == 0){
                Serial.println( F("Irrigate if It's Drier Than: (%, 100 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigTrigHum, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
		#endif
        if( command == 36 ){ // Irrigation On Daytime only
            if(cliStep == 0){
                Serial.println( F("Irrigate at Daytime Only: [0] OFF [1] ON") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigLimDay, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
        if( command == 37 ){ // Don't Irrigate if It's Rainy
            if(cliStep == 0){
                Serial.println( F("Don't Irrigate if It's Rainy: [0] OFF [1] ON") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigLimRin, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
        if( command == 38 ){
            if(cliStep == 0){
                Serial.println( F("Maximum Allowable Soil Moisture: (%, 0 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigLimSmt, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
        if( command == 39 ){
            if(cliStep == 0){
                Serial.println( F("Normal Duration for Each Irrigation: (Minutes, 0 -> NULL)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigLimTim, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
        if( command == 40 ){ // Grow Light Intervals
            if(cliStep == 0){
                Serial.println( F("Delay Between Each Grow Light Supplement: (Max 255, 0 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                Serial.println( F("[0] Minutes [1] Hours [2] Days [3] Weeks [4] Months?") );
                EEPROM.update(EEGrowTrigInt, inputString.toInt() );
                cliStep = 2;
            } else if (cliStep == 2) {
                EEPROM.update(EEGrowIntUnit, inputString.toInt() );
                cliStep = 0;
                if( EEPROM.read(EENextGrowMonth) > 12 || EEPROM.read(EENextGrowMonth) < 1 ){ inputString = "41"; } else { inputString = "25"; };
                return;
            }
        }
        if( command == 41 ){ // Next Run?
            if( (int)EEPROM.read(EEGrowTrigInt) == 0 ) { // Bounded to [32]
                Serial.println("Disabled! Take a look at [40] Scheduled Task...");
                cliStep == 0;
            } else {
                if(cliStep == 0){
                    Serial.println( F("Next Task: Year? (Two Digits: 2018 -> 18)") );
                    cliStep = 1;
                } else if(cliStep == 1){
                    EEPROM.update(EENextGrowYear, inputString.toInt() );
                    Serial.println( F("Month? (1 - 12)") );
                    cliStep = 2;
                } else if (cliStep == 2) {
                    Serial.println( F("Day? (1 - 31)") );
					EEPROM.update(EENextGrowMonth, inputString.toInt() );
                    cliStep = 3;
                } else if (cliStep == 3) {
                    Serial.println( F("Hour? (0 - 23)") );
                    EEPROM.update(EENextGrowDay, inputString.toInt() );
                    cliStep = 4;
                } else if (cliStep == 4) {
                    Serial.println( F("Minute? (0 - 59)") );
                    EEPROM.update(EENextGrowHour, inputString.toInt() );
                    cliStep = 5;
                } else if (cliStep == 5) {
					EEPROM.update(EENextGrowMinute, inputString.toInt() );
                    cliStep = 0;
                    //inputString = "25";
                    if( EEPROM.read(EEGrowEachDuration) > 0 ){ inputString = "25"; } else { inputString = "42"; };
                    return;
                }
            }
        }
        if( command == 42 ){ // Grow Light Dosage
            if(cliStep == 0){
                Serial.println( F("Each Task Duration: (Minutes, 0 -> NULL)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEGrowEachDuration, inputString.toInt() );
                cliStep = 0;
                //inputString = "25";
                if( EEPROM.read(EEGrowTrigMax) > 0 ){ inputString = "25"; } else { inputString = "44"; };
                return;
            }
        }
        if( command == 43 ){
            if(cliStep == 0){
                Serial.println( F("Turn On Grow Light Supply if It's Darker Than: (%, 100 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEGrowTrigMin, inputString.toInt() );
                cliStep = 0;
                inputString = "25";
                return;
            }
        }
        if( command == 44 ){
            if(cliStep == 0){
                Serial.println( F("Turn Off Grow Light Supply if It's Brighter Than: (%, 0 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEGrowTrigMax, inputString.toInt() );
                cliStep = 0;
                inputString = "25";
                return;
            }
        }
		if( command == 45 ){
            if(cliStep == 0){
                Serial.println( F("Don't Supply Grow Light After: (0-23 O'Clock, 24 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEGrowLightSupplyBefore, inputString.toInt() );
                cliStep = 0;
                inputString = "25";
                return;
            }
        }
		#ifdef _DHT
        if( command == 46 ){ // Keep Temperature About...
            if(cliStep == 0){
                Serial.println( F("Keep Temperature About: ('C, 0 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                Serial.println( F("...& for Night-time? ('C, 0 -> OFF) ") );
                EEPROM.update(EETempHumCTemp, inputString.toInt() );
                cliStep = 2;
            } else if (cliStep == 2) {
                Serial.println( F("Tolerance?") );
                EEPROM.update(EETempHumCTempNight, inputString.toInt() );
                cliStep = 3;
            } else if (cliStep == 3) {
                EEPROM.update(EETempHumCTTol, inputString.toInt() );
                cliStep = 0;
                inputString = "26";
                return;
            }
        }
        if( command == 47 ){ // Keep Humidity About...
            if(cliStep == 0){
                Serial.println( F("Keep Humidity About: (%, 0 -> OFF)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                Serial.println( F("...& for Night-time? (%, 0 -> OFF) ") );
                EEPROM.update(EETempHumCHum, inputString.toInt() );
                cliStep = 2;
            } else if (cliStep == 2) {
                Serial.println( F("Tolerance?") );
                EEPROM.update(EETempHumCHumNight, inputString.toInt() );
                cliStep = 3;
            } else if (cliStep == 3) {
                Serial.println( F("Use Fan/Cooler to Reduce Humidity: [1] YES [2] NO") );
                EEPROM.update(EETempHumCHTol, inputString.toInt() );
                cliStep = 4;
            } else if (cliStep == 4) {
                EEPROM.update(EEFANCforDeHumid, inputString.toInt() );
                cliStep = 0;
                inputString = "26";
                return;
            }
        }
		#endif
        #ifdef _Mixer
        if( command == 48 ){ // Fertilizer Rate
            if(cliStep == 0){
                Serial.println( F("How Much? (%)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEFeedFrtlzRate, inputString.toInt() );
                cliStep = 0;
                inputString = "27";
                return;
            }
        }
        if( command == 49 ){ // Fertilizer Pump Flow Rate
            if(cliStep == 0){
                Serial.println( F("Fertilizer Pump Flow Rate: (Liter/Sec)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEFeedFrtlzFlow, inputString.toInt() );
                cliStep = 0;
                inputString = "27";
                return;
            }
        }
        if( command == 50 ){ // Solution Container Volume
            if(cliStep == 0){
                Serial.println( F("Solution Container Volume: (Liter)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEFeedContainer, inputString.toInt() );
                cliStep = 0;
                inputString = "27";
                return;
            }
        }
        if( command == 51 ){ // Solution Container Volume
            if(cliStep == 0){
                Serial.println( F("Maximum Irrigation per Day: (Minutes, 0 -> Null)") );
                cliStep = 1;
            } else if (cliStep == 1) {
                EEPROM.update(EEIrigMaxpDay, inputString.toInt() );
                cliStep = 0;
                inputString = "24";
                return;
            }
        }
        #endif
        // Action
        if( command == 5 ){
            // Time & Date
            printDateTime();
        }
        if( command == 6 ){
			#ifdef _DHT
                // Humidity & Temperature
                DHTRead(0);
                // Check if any reads failed and exit early (to try again).
                // print Temperature & Humidity
                Serial.print( F("Humidity [%]: ") );  Serial.println(humid);
                Serial.print( F("Temperature ['C]: ") );  Serial.println(temp);
			#else
				Serial.println( F("UNAVAILABLE (COMPILED WITHOUT DHT)") );
			#endif
        }
        if( command == 7 ){
            // Day Light Intensity
            // Reading Light (LDR)
			int status = dayTime(); // --> Does Ldr(); itself.
            Serial.print( F("Day Light [%]: ") );  Serial.print(lux);
            if( status == 1 ){
                Serial.println( F(" (Night)") );
            } else if( status == 2 ) {
                Serial.println( F(" (Dusk/Dawn)") );
            } else if ( status == 3 ){
                Serial.println( F(" (Day)") );
            }
        }
        if( command == 8 ){
            // Soil Moisture
            Moist();
            if (smt_status >= (DENOISEA-1)){
              Serial.print( F("Soil Mositure [%]: ") );  Serial.println(moist);
              smt_reset();
            } else { Moist(); return; }
        }
        if( command == 9 ){
            // Food Tank
            int status = Food();
            if (fdt_status >= (DENOISEA-1)){
              Serial.print(food);
              if( status == 1 ){
                  Serial.println( F("-> FT Minimum.") );
              } else if( status == 2 ) {
                  Serial.println( F("-> FT Normal.") );
              } else if ( status == 3 ){
                  Serial.println( F("-> FT Maximum.") );
              } else {
                  Serial.println( F("-> Ambiguous!") );
  			      }
              fdt_reset();
          } else { Food(); return; }
        }
		if( command == 10  ){ if ( itsRainy() == true  ){ Serial.println( F("It's Rainy") ); } else { Serial.println( F("Dry") );} };
        // Calibration
        if( command == 11  ){
          if (ldr_status >= (DENOISEA-1)){
            EEPROM.update(EELdrMin, Ldr()); Serial.print(EEPROM.read(EELdrMin)); Serial.println( F("-> LDR.Min")  );
            ldr_reset();
          } else { Ldr(); return; }
        } // Minimum Light
        if( command == 12  ){
          if (ldr_status >= (DENOISEA-1)){
            EEPROM.update(EELdrMax, Ldr()); Serial.print(EEPROM.read(EELdrMax)); Serial.println( F("-> LDR.Max")  );
            ldr_reset();
          } else { Ldr(); return; }
        } // Maximum Light
        if( command == 13  ){
          if (smt_status >= (DENOISEA-1)){
            EEPROM.update(EESmtMin, Moist()); Serial.print(EEPROM.read(EESmtMin)); Serial.println( F("-> SM.Min")  );
            smt_reset();
          } else { Moist(); return; }
        } // Minimum Soil Moisture
        if( command == 14  ){
          if (smt_status >= (DENOISEA-1)){
            EEPROM.update(EESmtMax, Moist()); Serial.print(EEPROM.read(EESmtMax)); Serial.println( F("-> SM.Max")  );
            smt_reset();
          } else { Moist(); return; }
        } // Maximum Soil Moisture
        if( command == 15 ){
          Food();
          if (fdt_status >= (DENOISEA-1)) {
            EEPROM.update(EEFscMin,  food); Serial.print(EEPROM.read(EEFscMin)); Serial.println( F("-> FT.Min")  );
            fdt_reset();
          } else { Food(); return; }
        } // Minimum Detector in Food Tank
        if( command == 16 ){
          Food();
          if (fdt_status >= (DENOISEA-1)) {
            EEPROM.update(EEFscMax,  food); Serial.print(EEPROM.read(EEFscMax)); Serial.println( F("-> FT.Max")  );
            fdt_reset();
          } else { Food(); return; }
        } // Maximum Detector in FT
        // Diagnostics
        if( command == 17 ){ diagTimekeeper(); digitalWrite(IRIG, HIGH); Serial.println( F("Irrigation: ON") ); } // Irrigation
        if( command == 18 ){ diagTimekeeper(); digitalWrite(GROW, HIGH); Serial.println( F("Grow Light: ON") ); } // Grow Light
        if( command == 19 ){ diagTimekeeper(); digitalWrite(FANC, HIGH); Serial.println( F("Fan/Cooler: ON") ); } // Fan/Cooler
        if( command == 20 ){ diagTimekeeper(); digitalWrite(HEAT, HIGH); Serial.println( F("Heater: ON") ); } // Heater
        if( command == 21 ){ diagTimekeeper(); digitalWrite(FOGG, HIGH); Serial.println( F("Humidifier: ON") ); } // Humidifier
        if( command == 22 ){ // Stop
			stopDiagAll();
        }
        if( command == 99 ){ // Toggle "parametering"
			if( parametering == false ) {
				stopDiagAll();
				parametering = true;
				Serial.println("All Functions Are Disabled.\r\nType [99] to Toggle it back.");
			} else {
				parametering = false;
				Serial.println("Manual RESET Recommended.");
			}
        }
        // cleansing...
		//Serial.flush();
        inputString = "";
        stringComplete = false;
    }
}
void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n' || inChar == '\r') {
            stringComplete = true;
        } else {
			stringComplete = false;
		}
    }
}
