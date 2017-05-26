/*
 * Melon Green v1.0.0
 * Morteza H. Golkar
 * 2017
 * -----------------------------
 * Essential Functions
 * -----------------------------
*/

void initializePresets(){
    if ( (int)EEPROM.read(EEPROMCHK) != 123 ){ // EMPTY MEMORY (FIRST RUN)
        #ifdef _Serial
            Serial.println( F("-------------------") );
            Serial.print( F("Installing (EEPROM) Presets ") );
        #endif        
        EEPROM.write(EEPROMCHK, byte(123));
        // Colibration
        EEPROM.write(EELdrMin,  calibLDRMin);
        EEPROM.write(EELdrMax,  calibLDRMax);
        EEPROM.write(EESmtMin,  calibSmtMin);
        EEPROM.write(EESmtMax,  calibSmtMax);
        EEPROM.write(EEFscMin,  calibFdTMin);
        EEPROM.write(EEFscMax,  calibFdTMax);
        // EEPROM.write(EEFscRain, calibFdTRain); //-> old version
        // Settings - Time, Date & Regional
        //Duty Cycle
        EEPROM.write(EEDutyCycle, DutyCycle);
        EEPROM.write(EEDutyCycleUnit, DutyCycleUnit);
        EEPROM.write(EEDutyCycleWarmUp, DutyCycleWarmUp);
        EEPROM.write(EEDayLightTresh, DayLightThreshold);
        EEPROM.write(EENightLightTresh, NightLightThreshold);
        /*
        EEPROM.write(EELatitude,   latitude);
        EEPROM.write(EELongitude,  longitude);
        EEPROM.write(EEUTCMinutes, time_zone);
        EEPROM.write(EEQuadrant,   quadrant);
		// --> Reserved for Further Use
		*/
        // Settings - Irrigation
        EEPROM.write(EEIrigTrigSmt, IrrigateOnSoilMoistureBelow);
        EEPROM.write(EEIrigTrigInt, IrrigationOnIntervalsEvery);
        EEPROM.write(EEIrigIntUnit, IrrigateIntervalUnit);
        EEPROM.write(EEIrigTrigTmp, IrrigationOnTemperatureHigherThan);
        EEPROM.write(EEIrigTrigHum, IrrigationOnHumidityBelow);
        EEPROM.write(EEIrigLimDay,  IrrigateAtDaytimeOnly);
        EEPROM.write(EEIrigLimRin,  DontIrrigateOnRainyDays);
        EEPROM.write(EEIrigLimSmt,  IrrigateToMaximumSoilMoisture);
        EEPROM.write(EEIrigLimTim,  IrrigationDurationEachTime);
        // Settings - Grow Light Supplier
        EEPROM.write(EEGrowTrigInt, GrowLightIntervalsEvery);
        EEPROM.write(EEGrowIntUnit, GrowLightIntervalUnit);
        EEPROM.write(EEGrowEachDuration, GrowLightSupplementDurationEachTask);
        EEPROM.write(EEGrowTrigMin, GrowLightOnIfBelow);
        EEPROM.write(EEGrowTrigMax, GrowLightOFFIfAbove);
        EEPROM.write(EEGrowLightSupplyBefore, DontSupplyGrowLightAfter);
        // Settings - Temperature & Humidity
        EEPROM.write(EETempHumCTemp, KeepTemperatureAbout);
        EEPROM.write(EETempHumCTTol, TemperatureTolerance);
        EEPROM.write(EETempHumCHum, keepHumidityAbout);
        EEPROM.write(EETempHumCHTol, HumidityTolerance);
        EEPROM.write(EEFANCforDeHumid, UseFanCoolerToReduceHumidity);
        // Settings - Feeding Solution Mixer
        EEPROM.write(EEFeedFrtlzRate, FertilizerRate);
        EEPROM.write(EEFeedFrtlzFlow, FertilizerPumpFlowRate);
        EEPROM.write(EEFeedContainer, SolutionContainerVolume);
        #ifdef _Serial
            Serial.println( F("[OK]") );
        #endif
    }
}
void setupPinout(){
    pinMode(SVCC, OUTPUT);
    pinMode(IRIG, OUTPUT);
    pinMode(GROW, OUTPUT);
    pinMode(FANC, OUTPUT);
    pinMode(HEAT, OUTPUT);
    pinMode(FOGG, OUTPUT);
    pinMode(FRTL, OUTPUT);
    pinMode(WATR, OUTPUT);
    pinMode(RAIN, INPUT);
    pinMode(DEBM, INPUT);
}
void initializeModules(){
    // Temperature & Humidity
    #ifdef _DHT
        dht.begin();
    #endif
    // Time
    #ifdef _RTC
        if ( !rtc.begin() ) {
			#ifdef _Serial
            Serial.println( F("Couldn't find RTC\r\n-> Switched to Internal Clock [Inaccurate].\r\nCheck Time Settings PLZ.") );
			#endif
            INTERNAL_CLOCK = true;
            //return;
        } else if (rtc.lostPower()) {
			#ifdef _Serial
            Serial.println( F("RTC lost! Please Check Date & Time Settings.") );
			#endif
			rtc.adjust(DateTime(__DATE__, __TIME__));
        }
    #else
        INTERNAL_CLOCK = true;
        #ifdef _Serial
            Serial.println( F("System Reset! Please Check Your Settings.") );
        #endif
    #endif
    if( INTERNAL_CLOCK == true ){
        // last saved time stamp --> Now
		setTime((int)EEPROM.read(EEHour), (int)EEPROM.read(EEMinute), (int)EEPROM.read(EESecond), (int)EEPROM.read(EEDay), (int)EEPROM.read(EEMonth), (int)EEPROM.read(EEYear));
    }
    #ifdef _Serial
        Serial.println("-------------------");
    #endif
}
time_t updateNow(){  // Updates: int Now[6]...
    #ifdef _RTC
        DateTime current;
        if(INTERNAL_CLOCK == false){
            current = rtc.now();
            Now[0] = current.year() - century;
            Now[1] = current.month();
            Now[2] = current.day();
            Now[3] = current.hour();
            Now[4] = current.minute();
            Now[5] = current.second();
        } else {
    #endif
            Now[0] = year() - century;
            Now[1] = month();
            Now[2] = day();
            Now[3] = hour();
            Now[4] = minute();
            Now[5] = second();
    #ifdef _RTC
        }
	#else
		#ifdef _Serial
        if(command != 30) { // Don't Update EEPROM with Wrong Data accidentally while [Serial] Manual Adjustment
		#endif
            EEPROM.update(EEYear, Now[0]);
            EEPROM.update(EEMonth, Now[1]);
            EEPROM.update(EEDay, Now[2]);
            EEPROM.update(EEHour, Now[3]);
            EEPROM.update(EEMinute, Now[4]);
            EEPROM.update(EESecond, Now[5]);
		#ifdef _Serial
        }
		#endif
    #endif
    // Return Unixtimestamp
    #ifdef _RTC
        if(INTERNAL_CLOCK == false){
            return current.unixtime();
        } else {
    #endif
            return now();
    #ifdef _RTC
        }
    #endif
}
#ifdef _Serial
void printNextIrrigation(){
    Serial.print((century + (int)EEPROM.read(EENextIrigYear))); Serial.print("/");
    Serial.print( (int)EEPROM.read(EENextIrigMonth) ); Serial.print("/");
    Serial.print( (int)EEPROM.read(EENextIrigDay) ); Serial.print(" [");
    Serial.print( (int)EEPROM.read(EENextIrigHour) ); Serial.print(":");
    Serial.print( (int)EEPROM.read(EENextIrigMinute) ); Serial.println("]");
}
void printNextGrowLight(){
    Serial.print((century + (int)EEPROM.read(EENextGrowYear))); Serial.print("/");
    Serial.print( (int)EEPROM.read(EENextGrowMonth) ); Serial.print("/");
    Serial.print( (int)EEPROM.read(EENextGrowDay) ); Serial.print(" [");
    Serial.print( (int)EEPROM.read(EENextGrowHour) ); Serial.print(":");
    Serial.print( (int)EEPROM.read(EENextGrowMinute) ); Serial.println("]");
}
void printDateTime(){
    updateNow();
    Serial.print((Now[0] + century)); Serial.print("/");
    Serial.print(Now[1]); Serial.print("/");
    Serial.print(Now[2]); Serial.print(" [");
    Serial.print(Now[3]); Serial.print(":");
    Serial.print(Now[4]); /* Serial.print(":");
    Serial.print(Now[5]); */ Serial.println("]");
}
#endif
void stopDiagAll(){
    digitalWrite(IRIG, LOW);
    digitalWrite(GROW, LOW);
    digitalWrite(FANC, LOW);
    digitalWrite(HEAT, LOW);
    digitalWrite(FOGG, LOW);
    diag = false;
    #ifdef _Serial
        Serial.println( F("Manual Diagnostics: ALL OFF / TIME OUT") );
    #endif
}
void diagTimekeeper(){
    if (diag == false) {
        diagTime = millis(); delay(1);
        diag = true;
    } else {
        if ( ( millis() - diagTime ) >= DiagMaxMilliSecond) {
            stopDiagAll();  // Finish			
        }
    }
}
// Read Get Set -------------------------------
int Ldr(){
    int input = analogRead(LDR); delay(ANALOGDELAY);
    int result = map(input, 0, 1023, 0, 255);
    lux = map(result, EEPROM.read(EELdrMin), EEPROM.read(EELdrMax), 0, 100);  
    return result;
}
int Moist(){
    int input = analogRead(SOILMOISTURE); delay(ANALOGDELAY);
    int result = map(input, 0, 1023, 0, 255);
    moist = map(result, EEPROM.read(EESmtMin), EEPROM.read(EESmtMax), 0, 100);	 
    return result;
}
int Food() {
    int input = analogRead(FOODTANK); delay(ANALOGDELAY);
    food = map(input, 0, 1023, 0, 255);
    if( (food < EEPROM.read(EEFscMin)) && ((EEPROM.read(EEFscMin) - food) > calibTolerance) ){
        return 1; // Less Than Minimum
    } else if( (food >= (EEPROM.read(EEFscMin) - calibTolerance) ) && (food < (EEPROM.read(EEFscMax) - calibTolerance) ) ) {
        return 2; // Normal
    } else if ( (food > (EEPROM.read(EEFscMax) - calibTolerance)) /*&& (food < (EEPROM.read(EEFscRain) - calibTolerance) ) */ ){
        return 3; // Max
    }
    /* // -> old version
	 else if ( (food > (EEPROM.read(EEFscRain) - calibTolerance))  ){
		 return 4; // Rain
	 }
	 */
    else {
        return 0; // Ambiguous
    }
}
#ifdef _DHT
void DHTRead(int which){
    delay(DHTDELAY); // Wait a few moments between measurements.
    if(which == 1 || which == 0) { temp =  dht.readTemperature(); delay(DHTDELAY); }
    if(which == 2 || which == 0) { humid = dht.readHumidity(); delay(DHTDELAY); }
    if ( isnan(humid) && isnan(temp) ) {
        #ifdef _Serial
            Serial.println( F("Failed to read from DHT sensor!") );
        #endif
        return;
    }
}
#endif
int dayTime(){
    Ldr();
    if ( lux < (int)EEPROM.read(EENightLightTresh) ) {
        return 1; // Night
    } else if ( (lux >= (int)EEPROM.read(EENightLightTresh)) && (lux <= (int)EEPROM.read(EEDayLightTresh)) ) {
        return 2; // Sunday | Sunrise
    } else {
        return 3; // Day
    }
}
bool itsRainy(){
    // if( Food() == 4 ) { return true; } else { return false; } --> old Version	 
    bool rainy = false;
    if( digitalRead(RAIN) == HIGH ){ rainy = true; } else { rainy = false;}
    return rainy;
}
// Helpers
time_t stampUnix(int Sec, int Min, int Hour, int Day, int Mon, int Year){
    tmElements_t tmp;
    time_t stamp;
    tmp.Second = Sec;
    tmp.Minute = Min;
    tmp.Hour = Hour;
    tmp.Day = Day;
    tmp.Month = Mon;
    tmp.Year = Year - 1970;
    stamp =  makeTime(tmp);
    return stamp;
}
// Event Handling ---------------------------------
void setNextEvent(int irriGrow){
    int newMinute; int newHour; int newDay; int newMonth; int newYear; int interval; int unit;
    if ( irriGrow == 1 ) {
        // Next Irrigation
        newMinute = (int)EEPROM.read(EENextIrigMinute);
        newHour = (int)EEPROM.read(EENextIrigHour);
        newDay = (int)EEPROM.read(EENextIrigDay);
        newMonth = (int)EEPROM.read(EENextIrigMonth);
        newYear = (int)EEPROM.read(EENextIrigYear);
        interval = (int)EEPROM.read(EEIrigTrigInt);
        unit = (int)EEPROM.read(EEIrigIntUnit);
    } else {
        // Next Grow Light Supplement
        newMinute = (int)EEPROM.read(EENextGrowMinute);
        newHour = (int)EEPROM.read(EENextGrowHour);
        newDay = (int)EEPROM.read(EENextGrowDay);
        newMonth = (int)EEPROM.read(EENextGrowMonth);
        newYear = (int)EEPROM.read(EENextGrowYear);
        interval = (int)EEPROM.read(EEGrowTrigInt);
        unit = (int)EEPROM.read(EEGrowIntUnit);
    }
    if( unit == 0 ) {
        newMinute += interval; 
    } else if( unit == 1 ) {
        newHour += interval; 
    } else if( unit == 2 ) {
        newDay += interval;
    } else if( unit == 3 ) {
        newDay += (interval * 7);
    } else if( unit == 4 ) {
        newMonth += interval;
    }
    while( newMinute > 59 ){
        newMinute -= 60;
        newHour += 1;
    }
    while( newHour > 23 ){
        newHour -= 24;
        newDay += 1;
    }	
    while(newDay > 28) {
        if( ((newYear % 4) == 0) && (!(((newYear % 400) != 0) && ((newYear % 100) == 0))) ){
            /* leap year */
            if( (newMonth == 2) && (newDay > 29) ){
                newDay -= 29;
                newMonth += 1;
            }
        } else {
            /* common year */			
            if( newMonth == 2){ /* February */
                newDay -= 28;
                newMonth += 1;
            } else { /* 30-31Days Month*/
                if( ((newMonth <= 7) &&  ((newMonth % 2) == 1)) || ((newMonth > 7) &&  ((newMonth % 2) == 0)) ){
                    /* 31 days */
                    if (newDay > 31){
                        newDay -= 31;
                        newMonth += 1;	
                    } else { break; }
                } else {
                    /* 30 days */
                    if (newDay > 30){
                        newDay -= 30;
                        newMonth += 1;	
                    } else { break; }
                }			
            }
        }
    }	
    while( newMonth > 12 ){
        newHour -= 12;
        newYear += 1;
    }
    if ( irriGrow == true ) {
        // Update Next Irrigation
        EEPROM.update(EENextIrigMinute, newMinute);
        EEPROM.update(EENextIrigHour, newHour);
        EEPROM.update(EENextIrigDay, newDay);
        EEPROM.update(EENextIrigMonth, newMonth);
        EEPROM.update(EENextIrigYear, newYear);
    } else {
        // Update Next Grow Light Supplement
        EEPROM.update(EENextGrowMinute, newMinute);
        EEPROM.update(EENextGrowHour, newHour);
        EEPROM.update(EENextGrowDay, newDay);
        EEPROM.update(EENextGrowMonth, newMonth);
        EEPROM.update(EENextGrowYear, newYear);
    }
}
void careLostEvents(int irriGrow) {
    time_t nowTimeStamp = updateNow();
    time_t nextTimeStamp;
    #ifdef _Serial
        bool changed = false;
    #endif
    do {
        if(irriGrow == 1) {
            nextTimeStamp = stampUnix(0, (int)EEPROM.read(EENextIrigMinute), (int)EEPROM.read(EENextIrigHour), (int)EEPROM.read(EENextIrigDay), (int)EEPROM.read(EENextIrigMonth), (century + (int)EEPROM.read(EENextIrigYear) ) );
        } else {
            nextTimeStamp = stampUnix(0, (int)EEPROM.read(EENextGrowMinute), (int)EEPROM.read(EENextGrowHour), (int)EEPROM.read(EENextGrowDay), (int)EEPROM.read(EENextGrowMonth), (century + (int)EEPROM.read(EENextGrowYear) ) );
        }
        if ( nowTimeStamp >  nextTimeStamp ){
            setNextEvent(irriGrow);
            #ifdef _Serial
            changed = true;
            #endif
        }
    } while( nowTimeStamp >  nextTimeStamp );
    #ifdef _Serial
        if (changed == true) {
            Serial.println( F("-------------------") );
            Serial.print( F("Some Scheduled Tasks Lost! -> Next ") );
            if (irriGrow == true) {
                Serial.print( F("IRRIGATION: ") ); 
                printNextIrrigation();
            } else {
                Serial.print( F("GROW LIGHT: ") ); 
                printNextGrowLight();
            }
        }
    #endif
}
// Irrigation ---------------------------------
bool scheduledIrrigationNow(){
	if ( (int)EEPROM.read(EEIrigTrigInt) == 0 ) { return false; }
    careLostEvents(1); //--> It Does 'updateNow();'
    if ((int)EEPROM.read(EENextIrigYear) == Now[0] && (int)EEPROM.read(EENextIrigMonth) == Now[1] && (int)EEPROM.read(EENextIrigDay) == Now[2] && (int)EEPROM.read(EENextIrigHour) == Now[3] && (int)EEPROM.read(EENextIrigMinute) == Now[4] ) { return true; } else { return false; }
}
bool limitIrrigation(){
    bool shallwe = false;
    if ( ( IrrigationTime != 0 ) && ( (( millis() - IrrigationTime ) / 60000) >= (int)EEPROM.read(EEIrigLimTim) ) ) {
        shallwe = true;	
    } else if( ((int)EEPROM.read(EEIrigLimDay) != 0) && (dayTime() < 2) ) {
        shallwe = true;
    } else if( ((int)EEPROM.read(EEIrigLimRin) != 0) && (itsRainy() == true) ) {
        shallwe = true;			
    } else {
        Moist();
        if ( moist >= (int)EEPROM.read(EEIrigLimSmt) && ((int)EEPROM.read(EEIrigLimSmt) > 0) ) {
            shallwe = true;
        }				
    }	
    if ( shallwe == true && is_irrigating == true) {
        is_irrigating = false;
        IrrigationTime = 0;
        digitalWrite(IRIG, LOW);
        #ifdef _Serial
            Serial.println( F("Irrigation: OFF") );
        #endif
    }
    return shallwe;
}
void checkIrrigationJob(){
    bool shallwe = false;
    #ifdef _DHT
    if( ((int)EEPROM.read(EEIrigTrigHum) < 100) || ((int)EEPROM.read(EEIrigTrigTmp) > 0) ) {
        DHTRead(0);
        if ( (temp  > (int)EEPROM.read(EEIrigTrigTmp)) || (humid < (int)EEPROM.read(EEIrigTrigHum)) ) { shallwe = true; }
    }
    #endif
    if( (int)EEPROM.read(EEIrigTrigSmt) < 100 ) {
        Moist();
        if( moist < (int)EEPROM.read(EEIrigTrigSmt) ) { shallwe = true; }
    }
    if( scheduledIrrigationNow() == true ) {
        shallwe = true;
        setNextEvent(1); 
    }
    if( (shallwe == true) && (limitIrrigation() == false) ) {
        IrrigationTime = millis(); delay(1);
        is_irrigating = true;
        digitalWrite(IRIG, HIGH);
        #ifdef _Serial
        Serial.println( F("Irrigation: ON") );
        #endif
    }
}
// Grow Light Supplier ---------------------------------
bool scheduledGrowLightNow(){
	if ( (int)EEPROM.read(EEGrowTrigInt) == 0 ) { return false; }
    careLostEvents(2); //--> It Does 'updateNow();'
    if ((int)EEPROM.read(EENextGrowYear) == Now[0] && (int)EEPROM.read(EENextGrowMonth) == Now[1] && (int)EEPROM.read(EENextGrowDay) == Now[2] && (int)EEPROM.read(EENextGrowHour) == Now[3] && (int)EEPROM.read(EENextGrowMinute) == Now[4] ) { return true; } else { return false; }
}
bool limitGrowLight(){
    bool shallwe = false;
    if( ((int)EEPROM.read(EEGrowLightSupplyBefore) <= 23) && ( (Now[3] >= (int)EEPROM.read(EEGrowLightSupplyBefore)) || (dayTime() < 2) ) ){
        shallwe = true;	
    } else if( (int)EEPROM.read(EEGrowTrigMax) > 0 ){
		Ldr();
        if( lux > (int)EEPROM.read(EEGrowTrigMax) ) { shallwe = true; }
    } else if ( (( millis() - growLightTime ) / 60000) >= (int)EEPROM.read(EEGrowEachDuration) ) {
        shallwe = true;
    }
    if ( shallwe == true && is_supplyingGrowLight == true) {
        is_supplyingGrowLight = false;
        growLightTime = 0;
        digitalWrite(GROW, LOW);
        #ifdef _Serial
            Serial.println( F("GrowLightSupplier: OFF") );
        #endif
    }
    return shallwe;
}
void checkGrowLightJob(){
    updateNow();
    bool shallwe = false;
    if( ((int)EEPROM.read(EEGrowLightSupplyBefore) > 23) || ( (Now[3] < (int)EEPROM.read(EEGrowLightSupplyBefore)) && (dayTime() > 1) ) ){
        if( scheduledGrowLightNow() == true ) {
            shallwe = true;
            setNextEvent(2); 
        } else if ( (int)EEPROM.read(EEGrowTrigMin) < 99 ) {
            Ldr();
            if( lux < (int)EEPROM.read(EEGrowTrigMin) ){ shallwe = true; }
        }
    }
    if( (shallwe == true) && (limitGrowLight() == false) ) {
        growLightTime = millis(); delay(1);
        is_supplyingGrowLight = true;
        digitalWrite(GROW, HIGH);
        #ifdef _Serial
            Serial.println( F("GrowLightSupplier: ON") );
        #endif
    }
}
// Temperature & Humidity Controllers ---------------------------------
#ifdef _DHT
void temperatureKeeper(bool awk){
    if(awk == true){
        if( (int)EEPROM.read(EETempHumCTemp) > 0 ){
            DHTRead(1);
            if( is_cooling == false && temp > ( (int)EEPROM.read(EETempHumCTemp) + ((int)EEPROM.read(EETempHumCTTol)/2) ) ){ // Cool
                digitalWrite(HEAT, LOW);
                digitalWrite(FANC, HIGH);
                is_heating = false; is_cooling = true;
                #ifdef _Serial
                    Serial.println( F("COOLING...") );
                #endif
            } else if( is_heating == false && temp < ( (int)EEPROM.read(EETempHumCTemp) - ((int)EEPROM.read(EETempHumCTTol)/2) ) ){ // Heat
                digitalWrite(HEAT, HIGH);
                digitalWrite(FANC, LOW);
                is_heating = true; is_cooling = false;			 
                #ifdef _Serial
                    Serial.println( F("HEATING...") );
                #endif
            } else if( is_heating == true || is_cooling == true ) {
                if( is_cooling == true && temp <= ( (int)EEPROM.read(EETempHumCTemp) + ((int)EEPROM.read(EETempHumCTTol)/2) ) ){
                    is_cooling = false;
                    if( is_Fanning == false ){
                        digitalWrite(FANC, LOW);
                        #ifdef _Serial
                            Serial.println( F("COOLER: OFF") );
                        #endif
                    }
                }
                if( is_heating == true && temp >= ( (int)EEPROM.read(EETempHumCTemp) - ((int)EEPROM.read(EETempHumCTTol)/2) ) ){
                    is_heating = false; 
                    digitalWrite(HEAT, LOW);
                    #ifdef _Serial
                        Serial.println( F("HEATER: OFF") );
                    #endif
                }
            }
        }
    }
}
void humidityKeeper(bool awk){
    if(awk == true){
        if( (int)EEPROM.read(EETempHumCHum) > 0 ){
            DHTRead(2);
            if( is_Fanning == false && humid > ( (int)EEPROM.read(EETempHumCHum) + ((int)EEPROM.read(EETempHumCHTol)/2) ) ){ // Fan
                digitalWrite(FOGG, LOW); is_Fogging = false;
                if( is_heating == false && (int)EEPROM.read(EEFANCforDeHumid) == 1 ){
                    digitalWrite(FANC, HIGH);
                    is_Fanning = true;
                    #ifdef _Serial
                        Serial.println( F("FAN (Humidity Reduction)...") );
                    #endif
                }
            } else if( is_Fogging == false && humid < ( (int)EEPROM.read(EETempHumCHum) - ((int)EEPROM.read(EETempHumCHTol)/2) ) ){ // Fogger
                digitalWrite(FOGG, HIGH); is_Fogging = true; is_Fanning = false;
                if(is_cooling == false){ digitalWrite(FANC, LOW); }									 
                #ifdef _Serial
                    Serial.println( F("FOGGER: ON") );
                #endif
            } else if (is_Fogging == true || is_Fanning == true) {
                if( is_Fanning == true && humid <= ( (int)EEPROM.read(EETempHumCHum) + ((int)EEPROM.read(EETempHumCHTol)/2) ) ){
                    is_Fanning = false;
                    if(is_cooling == false){
                        digitalWrite(FANC, LOW); 
                        #ifdef _Serial
                            Serial.println( F("FAN: OFF") );
                        #endif
                    }
                }
                if( is_Fogging == true && humid >= ( (int)EEPROM.read(EETempHumCHum) - ((int)EEPROM.read(EETempHumCHTol)/2) ) ){
                    digitalWrite(FOGG, LOW);
                    is_Fogging = false;
                    #ifdef _Serial
                        Serial.println( F("FOGGER: OFF") );
                    #endif
                }
            }
        }
    }
}
#endif
// Food Solution Tank/Mixer ---------------------------------
#ifdef _Mixer
void solutionMixer(bool awk){
    if(awk == true){
        if( (int)EEPROM.read(EEFeedFrtlzRate) > 0 && (int)EEPROM.read(EEFeedFrtlzFlow) > 0 && (int)EEPROM.read(EEFeedContainer) > 0 ){
            int tankStatus = Food(); // 2 -> Normal
            if( tankStatus < 2 && is_Mixing == false) {
                fertilizerTime = millis(); delay(1);
                // pause irrigation
                if(IrrigationTime > 0){
                    is_irrigating = false;
                    digitalWrite(IRIG, LOW);			
                    #ifdef _Serial
                        Serial.println( F("Irrigation Paused.") );
                    #endif
                }
                // add fertilizer
                digitalWrite(FRTL, HIGH);
                is_Mixing = true;
                #ifdef _Serial
                    Serial.println( F("+ Fertilizer...") );
                #endif
            } else if ( tankStatus < 2 && is_Mixing == true && fertilizer_added == false){
                unsigned int fertilizerDosageTime = ( (int)EEPROM.read(EEFeedContainer) / 100 * (int)EEPROM.read(EEFeedFrtlzRate) ) / (int)EEPROM.read(EEFeedFrtlzFlow);
                if( (fertilizerTime != 0) && (millis() - fertilizerTime) > (fertilizerDosageTime * 1000) ) {
                    digitalWrite(FRTL, LOW);
                    digitalWrite(WATR, HIGH);
                    fertilizer_added = true;				
                    #ifdef _Serial
                        Serial.println( F("+ Diluting Food Solution...") );
                    #endif
                } // -> Dilute with water
            } else if ( tankStatus > 2 && is_Mixing == true){			
                #ifdef _Serial
                    Serial.println( F("Solution Tank [OK].") );
                #endif
                digitalWrite(WATR, LOW);
                is_Mixing = false;
                fertilizer_added = false;
                // continue irrigation
                if(IrrigationTime > 0){
                    IrrigationTime = (millis() - (fertilizerTime - IrrigationTime));
                    is_irrigating = true;
                    digitalWrite(IRIG, HIGH);							
                    #ifdef _Serial
                        Serial.println( F("Continue Irrigation.") );
                    #endif
                }
                //
                fertilizerTime = 0;			
            }
        }
    }
}
#endif
// Duty Cycle -------------------------------------------
void dutyCycle(){
    if( (int)EEPROM.read(EEDutyCycle) == 0 || debugMode == true ){ // SleepLess
        digitalWrite(SVCC, HIGH);
        awake = true; warm = true;
    } else {
        if (awake == false && warm == false){
            // Calculatin Duty Period
            int unit = (int)EEPROM.read(EEDutyCycleUnit);
            unsigned long dutyPeriod;
            if(unit == 0){
                dutyPeriod = (1000 / (int)EEPROM.read(EEDutyCycle));
            } else if (unit == 1){
                dutyPeriod = (60000 / (int)EEPROM.read(EEDutyCycle));
            } else if (unit == 2){
                dutyPeriod = (3600000 / (int)EEPROM.read(EEDutyCycle));
            } else if (unit == 3){
                dutyPeriod = (86400000 / (int)EEPROM.read(EEDutyCycle));
            } else {
                EEPROM.update(EEDutyCycleUnit, 1);
                return;
            }
            // Call of Duty?
            if( (millis() - lastDuty) > dutyPeriod ){ warm = true; }
            if( scheduledIrrigationNow() == true || scheduledGrowLightNow() == true ){ warm = true; } // but a task!
            // Shall we warm up?
            if( warm == true ){
                digitalWrite(SVCC, HIGH);
                warmUpTime = millis(); delay(1);				 
            }
        } else if (awake == false && warm == true){
            if( (warmUpTime != 0) && (millis() - warmUpTime) > ((int)EEPROM.read(EEDutyCycleWarmUp) * 1000) ) { awake = true; lastDuty = millis(); }
        } else if (awake == true && warm == true) { // it's warm & awake...
            // time to sleep?
            if( is_irrigating == false && is_supplyingGrowLight == false && is_heating == false && is_cooling == false && is_Fanning == false && is_Fogging == false && is_Mixing == false){
                warm = false; awake = false;
                warmUpTime = 0;
                digitalWrite(SVCC, LOW);
            }
        }
    }	
}