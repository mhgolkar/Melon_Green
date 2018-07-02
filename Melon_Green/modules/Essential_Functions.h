/*
 * Melon Green v1.2.0
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
        EEPROM.write(EEIrigMaxpDay, MaximumIrrigationPerDay);
        // Settings - Grow Light Supplier
        EEPROM.write(EEGrowTrigInt, GrowLightIntervalsEvery);
        EEPROM.write(EEGrowIntUnit, GrowLightIntervalUnit);
        EEPROM.write(EEGrowEachDuration, GrowLightSupplementDurationEachTask);
        EEPROM.write(EEGrowTrigMin, GrowLightOnIfBelow);
        EEPROM.write(EEGrowTrigMax, GrowLightOFFIfAbove);
        EEPROM.write(EEGrowLightSupplyBefore, DontSupplyGrowLightAfter);
        // Settings - Temperature & Humidity
        EEPROM.write(EETempHumCTemp, KeepTemperatureAbout);
        EEPROM.write(EETempHumCTempNight, KeepTemperatureNight);
        EEPROM.write(EETempHumCTTol, TemperatureTolerance);
        EEPROM.write(EETempHumCHum, keepHumidityAbout);
        EEPROM.write(EETempHumCHumNight, keepHumidityNight);
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
        }  else if (! rtc.isrunning()) { // DS1307
			#ifdef _Serial
            Serial.println( F("RTC Error! Please Check Date & Time Settings.") );
			#endif
			//rtc.adjust(DateTime(__DATE__, __TIME__)); // RTC_DS3231
        }
        /* else if (rtc.lostPower()) { // RTC_DS3231
			#ifdef _Serial
            Serial.println( F("RTC lost! Please Check Date & Time Settings.") );
			#endif
			rtc.adjust(DateTime(__DATE__, __TIME__));
        } */
    #else
        INTERNAL_CLOCK = true;
        #ifdef _Serial
            Serial.println( F("System Reset or RTC Error! Please Check Sys & Your Settings.") );
        #endif
    #endif
    /*
    if( INTERNAL_CLOCK == true ){
        // last saved time stamp --> Now
		setTime((int)EEPROM.read(EEHour), (int)EEPROM.read(EEMinute), (int)EEPROM.read(EESecond), (int)EEPROM.read(EEDay), (int)EEPROM.read(EEMonth), (int)EEPROM.read(EEYear));
    }
    */
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
    /*
    // removed in 1.2.0 r2
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
    */
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
void delayF(unsigned long del) { // automatic adjusting internalclock after delays
  delay(del);
  forgotten += del;
  if (INTERNAL_CLOCK == true){
    if(forgotten > ADJUSTINT){
      // adjust time after x sec of delays
      updateNow();
      if (Now[4] > ADJUSTINT/60000){
        setTime(Now[3], ( Now[4] - (ADJUSTINT/60000) ), Now[5], Now[2], Now[1], Now[0]);
        forgotten = forgotten - ADJUSTINT;
      }
    }
  }
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
    Serial.print(Now[5]); */
    if(INTERNAL_CLOCK == false){
      Serial.println("] RTC");
    } else {
      Serial.println("] INTC");
    }
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
        diagTime = millis() - 1;
        diag = true;
    } else {
        if ( ( millis() - diagTime ) >= DiagMaxMilliSecond) {
            stopDiagAll();  // Finish
        }
    }
}
// Analog Sensors -------------------------------
  // LDR (Light Intensity)
void ldr_reset(){ ldr_status = 0; ldr_first_attempt = 0; ldr_bank = 0; }
int Ldr(int noise_reduction = 0){
    int input = 0;
    int get = analogRead(LDR); delayF(ANALOGDELAY);
    if(noise_reduction != 99){
      if(ldr_status == 0) ldr_first_attempt = millis() - 1;
      if( millis() >= (ldr_first_attempt + ((DENOISES * 1000 / DENOISEA) * ldr_status) ) ){
        ldr_bank += get;
        ldr_status++;
      }
      input = ldr_bank / ldr_status;
      if(ldr_status > DENOISEA) ldr_reset();
    } else {
      input = get;
    }
    int result = map(input, 0, 1023, 0, 255);
    lux = map(result, EEPROM.read(EELdrMin), EEPROM.read(EELdrMax), 0, 100);
    return result;
}
  // Soil Moisture
void smt_reset(){ smt_status = 0; smt_first_attempt = 0; smt_bank = 0; }
int Moist(int noise_reduction = 0){
    int input = 0;
    int get = analogRead(SOILMOISTURE); delayF(ANALOGDELAY);
    if(noise_reduction != 99){
      if(smt_status == 0) smt_first_attempt = millis() - 1;
      if( millis() >= (smt_first_attempt + ((DENOISES * 1000 / DENOISEA) * smt_status) ) ){
        smt_bank += get;
        smt_status++;
      }
      input = smt_bank / smt_status;
      if(smt_status > DENOISEA) smt_reset();
    } else {
      input = get;
    }
    int result = map(input, 0, 1023, 0, 255);
    moist = map(result, EEPROM.read(EESmtMin), EEPROM.read(EESmtMax), 0, 100);
    return result;
}
  // Food Tank Level
void fdt_reset(){ fdt_status = 0; fdt_first_attempt = 0; fdt_bank = 0; }
int Food(int noise_reduction = 0) {
    int input = 0;
    int get = analogRead(FOODTANK); delayF(ANALOGDELAY);
    if(noise_reduction != 99){
      if(fdt_status == 0) fdt_first_attempt = millis() - 1;
      if( millis() >= (fdt_first_attempt + ((DENOISES * 1000 / DENOISEA) * fdt_status) ) ){
        fdt_bank += get;
        fdt_status++;
      }
      input = fdt_bank / fdt_status;
      if(fdt_status > DENOISEA) fdt_reset();
    } else {
      input = get;
    }
    food = map(input, 0, 1023, 0, 255);
    if( food <= (int)EEPROM.read(EEFscMin) ){
        return 1; // Less Than Minimum
    } else if( food > (int)EEPROM.read(EEFscMin) && food < (int)EEPROM.read(EEFscMax) ) {
        return 2; // Normal
    } else if ( food >= (int)EEPROM.read(EEFscMax) ){
        return 3; // Max
    } else {
        return 0; // Ambiguous
    }
}
int dayTime(){
    Ldr(99);
    if ( lux < (int)EEPROM.read(EENightLightTresh) ) {
        return 1; // Night
    } else if ( (lux >= (int)EEPROM.read(EENightLightTresh)) && (lux <= (int)EEPROM.read(EEDayLightTresh)) ) {
        return 2; // Sunday | Sunrise
    } else {
        return 3; // Day
    }
}
// Digital Sensors
  // Digital Humidity & Temperature
#ifdef _DHT
void DHTRead(int which){
    delayF(DHTDELAY); // Wait a few moments between measurements.
    if(which == 1 || which == 0) { temp =  dht.readTemperature(); delayF(DHTDELAY); }
    if(which == 2 || which == 0) { humid = dht.readHumidity(); delayF(DHTDELAY); }
    if ( isnan(humid) && isnan(temp) ) {
        #ifdef _Serial
            Serial.println( F("Failed to read from DHT sensor!") );
        #endif
        return;
    }
}
#endif
bool itsRainy(){
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
    if ((int)EEPROM.read(EENextIrigYear) == Now[0] && (int)EEPROM.read(EENextIrigMonth) == Now[1] && (int)EEPROM.read(EENextIrigDay) == Now[2] && (int)EEPROM.read(EENextIrigHour) == Now[3] && (int)EEPROM.read(EENextIrigMinute) == Now[4] ) {
      return true;
    } else {
      careLostEvents(1); //--> It Does 'updateNow();'
      return false;
    }
}
bool limitIrrigation(){
    bool shallwe = false;
    if ( (IrrigationTime != 0) && (millis() - IrrigationTime) > ((int)EEPROM.read(EEIrigLimTim) * 60000) ) {
        shallwe = true;
    }
    if( !shallwe && ((int)EEPROM.read(EEIrigLimDay) != 0) && (dayTime() < 2) ) {
        shallwe = true;
    }
    if( !shallwe && ((int)EEPROM.read(EEIrigLimRin) != 0) && (itsRainy() == true) ) {
        shallwe = true;
    }
    if( !shallwe ){
        Moist();
        if (smt_status >= (DENOISEA-1)){
          if ( moist >= (int)EEPROM.read(EEIrigLimSmt) && ((int)EEPROM.read(EEIrigLimSmt) > 0) ) { shallwe = true; }
          smt_reset();
        }
    }
    if( !shallwe && (Total_Irrig_per_Day >= (int)EEPROM.read(EEIrigMaxpDay)) ){
      shallwe = true;
    }
    #ifdef _DHT
    // Temperature
    if( !shallwe && (int)EEPROM.read(EEIrigTrigTmp) > 0 ) {
        DHTRead(1);
        if ( temp  <= (int)EEPROM.read(EEIrigTrigTmp) ) { shallwe = true; }
    }
    // Humidity
    if( !shallwe && (int)EEPROM.read(EEIrigTrigHum) < 100 ) {
        DHTRead(2);
        if ( humid >= (int)EEPROM.read(EEIrigTrigHum) ) { shallwe = true; }
    }
    #endif
    updateNow();
    if( Today != Now[2] ){
      Total_Irrig_per_Day = 0;
      Today = Now[2];
    }
    if ( shallwe == true && is_irrigating == true) {
        is_irrigating = false;
        Total_Irrig_per_Day += (int)( (millis() - IrrigationTime) / 60000 );
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
    if ( limitIrrigation() == false ){
      if( (int)EEPROM.read(EEIrigTrigSmt) < 100 ) {
          Moist();
          if (smt_status >= (DENOISEA-1)){
            if( moist < (int)EEPROM.read(EEIrigTrigSmt) ) { shallwe = true; }
            smt_reset();
          }
      }
      if( scheduledIrrigationNow() == true ) {
          shallwe = true;
          if(awake == true) setNextEvent(1);
      }
      #ifdef _DHT
      // Temperature
      if( !shallwe && smt_status == 0 ){
        if( (int)EEPROM.read(EEIrigTrigTmp) > 0 ) {
            DHTRead(1);
            if ( temp  > (int)EEPROM.read(EEIrigTrigTmp) ) { shallwe = true; }
        } else if( (int)EEPROM.read(EEIrigTrigHum) < 100 ) {
            DHTRead(2);
            if ( humid < (int)EEPROM.read(EEIrigTrigHum) ) { shallwe = true; }
        }
      }
      #endif
    }
    if( shallwe == true ) {
        IrrigationTime = millis() - 1;
        is_irrigating = true;
        digitalWrite(IRIG, HIGH);
        #ifdef _Serial
        Serial.println( F("Irrigation: ON") );
        #endif
    }
}
// Grow Light Supplier ---------------------------------
bool scheduledGrowLightNow(){
  updateNow();
	if ( (int)EEPROM.read(EEGrowTrigInt) == 0 ) { return false; }
    if ((int)EEPROM.read(EENextGrowYear) == Now[0] && (int)EEPROM.read(EENextGrowMonth) == Now[1] && (int)EEPROM.read(EENextGrowDay) == Now[2] && (int)EEPROM.read(EENextGrowHour) == Now[3] && (int)EEPROM.read(EENextGrowMinute) == Now[4] ) {
      return true;
    } else {
      careLostEvents(2); //--> It Does 'updateNow();'
      return false;
    }
}
bool limitGrowLight(){
    bool shallwe = false;
    if( ((int)EEPROM.read(EEGrowLightSupplyBefore) <= 23) && ( (Now[3] >= (int)EEPROM.read(EEGrowLightSupplyBefore)) || (dayTime() < 2) ) ){
        shallwe = true;
    }
    if( (int)EEPROM.read(EEGrowTrigMax) > 0 ){
		    Ldr();
        if( ldr_status >= DENOISEA ) {
          if( lux > (int)EEPROM.read(EEGrowTrigMax) ) { shallwe = true; }
          ldr_reset();
        }
    }
    if ( growLightTime > 0 && ( millis() - growLightTime ) > ((int)EEPROM.read(EEGrowEachDuration) * 60000) ) {
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
    bool shallwe = false;
    if( limitGrowLight() == false ){
      updateNow();
      if( ((int)EEPROM.read(EEGrowLightSupplyBefore) > 23) || ( (Now[3] < (int)EEPROM.read(EEGrowLightSupplyBefore)) && (dayTime() > 1) ) ){
          if( scheduledGrowLightNow() == true ) {
              shallwe = true;
              growLightTime = millis() - 1;
              if(awake == true) setNextEvent(2);
          } else if ( (int)EEPROM.read(EEGrowTrigMin) < 99 ) {
              Ldr();
              if( ldr_status >= DENOISEA ){
                if( lux < (int)EEPROM.read(EEGrowTrigMin) ) { shallwe = true; }
                ldr_reset();
              }
          }
      }
    }
    if( shallwe == true ) {
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
        if( ( (int)EEPROM.read(EETempHumCTemp) > 0 && dayTime() >= 2 ) ||  ( (int)EEPROM.read(EETempHumCTempNight) > 0 && dayTime() < 2 ) ){
          int target_temp = 0;
          if ( dayTime() < 2 ){
            target_temp = (int)EEPROM.read(EETempHumCTempNight);
          } else {
            target_temp = (int)EEPROM.read(EETempHumCTemp);
          }
            DHTRead(1);
            if( is_cooling == false && temp > ( target_temp + ((int)EEPROM.read(EETempHumCTTol)/2) ) ){ // Cool
                digitalWrite(HEAT, LOW);
                digitalWrite(FANC, HIGH);
                is_heating = false; is_cooling = true;
                #ifdef _Serial
                    Serial.println( F("COOLING...") );
                #endif
            } else if( is_heating == false && temp < ( target_temp - ((int)EEPROM.read(EETempHumCTTol)/2) ) ){ // Heat
                digitalWrite(HEAT, HIGH);
                digitalWrite(FANC, LOW);
                is_heating = true; is_cooling = false;
                #ifdef _Serial
                    Serial.println( F("HEATING...") );
                #endif
            } else if( is_heating == true || is_cooling == true ) {
                if( is_cooling == true && temp <= ( target_temp + ((int)EEPROM.read(EETempHumCTTol)/2) ) ){
                    is_cooling = false;
                    if( is_Fanning == false ){
                        digitalWrite(FANC, LOW);
                        #ifdef _Serial
                            Serial.println( F("COOLER: OFF") );
                        #endif
                    }
                }
                if( is_heating == true && temp >= ( target_temp - ((int)EEPROM.read(EETempHumCTTol)/2) ) ){
                    is_heating = false;
                    digitalWrite(HEAT, LOW);
                    #ifdef _Serial
                        Serial.println( F("HEATER: OFF") );
                    #endif
                }
            }
        } else {
              // Temperature Controller not set but Devices are already ON, so we need to turn them OFF
            if( is_heating == true ){
              is_heating = false;
              digitalWrite(HEAT, LOW);
              #ifdef _Serial
                  Serial.println( F("HEATER: OFF") );
              #endif
            }
            if ( is_cooling == true ) {
              is_cooling = false;
              if( is_Fanning == false ) digitalWrite(FANC, LOW);
              #ifdef _Serial
                  Serial.println( F("COOLER: OFF") );
              #endif
            }
        }
    }
}
void humidityKeeper(bool awk){
    if(awk == true){
        if( ( (int)EEPROM.read(EETempHumCHum) > 0 && dayTime() >= 2 ) ||  ( (int)EEPROM.read(EETempHumCHumNight) > 0 && dayTime() < 2 ) ){
            int target_humid = 0;
            if ( dayTime() < 2 ){
              target_humid = (int)EEPROM.read(EETempHumCHumNight);
            } else {
              target_humid = (int)EEPROM.read(EETempHumCHum);
            }
            DHTRead(2);
            if( is_Fanning == false && humid > ( target_humid + ((int)EEPROM.read(EETempHumCHTol)/2) ) ){ // Fan
                digitalWrite(FOGG, LOW); is_Fogging = false;
                if( is_heating == false && (int)EEPROM.read(EEFANCforDeHumid) == 1 ){
                    digitalWrite(FANC, HIGH);
                    is_Fanning = true;
                    #ifdef _Serial
                        Serial.println( F("FAN (Humidity Reduction)...") );
                    #endif
                }
            } else if( is_Fogging == false && humid < ( target_humid - ((int)EEPROM.read(EETempHumCHTol)/2) ) ){ // Fogger
                digitalWrite(FOGG, HIGH); is_Fogging = true; is_Fanning = false;
                if(is_cooling == false){ digitalWrite(FANC, LOW); }
                #ifdef _Serial
                    Serial.println( F("FOGGER: ON") );
                #endif
            } else if (is_Fogging == true || is_Fanning == true) {
                if( is_Fanning == true && humid <= ( target_humid + ((int)EEPROM.read(EETempHumCHTol)/2) ) ){
                    is_Fanning = false;
                    if(is_cooling == false){
                        digitalWrite(FANC, LOW);
                        #ifdef _Serial
                            Serial.println( F("FAN: OFF") );
                        #endif
                    }
                }
                if( is_Fogging == true && humid >= ( target_humid - ((int)EEPROM.read(EETempHumCHTol)/2) ) ){
                    digitalWrite(FOGG, LOW);
                    is_Fogging = false;
                    #ifdef _Serial
                        Serial.println( F("FOGGER: OFF") );
                    #endif
                }
            }
        } else {
          // Humidity Controller not set but Devices are already ON, so we need to turn them OFF
          if( is_Fanning == true ){
              is_Fanning = false;
              if ( is_cooling == false ) digitalWrite(FANC, LOW);
              #ifdef _Serial
                  Serial.println( F("FAN: OFF") );
              #endif
          }
          if ( is_Fogging == true ){
              digitalWrite(FOGG, LOW);
              is_Fogging = false;
              #ifdef _Serial
                  Serial.println( F("FOGGER: OFF") );
              #endif
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
          if( fdt_status >= DENOISEA ){ // Noise Reduction
            if( tankStatus < 2 && is_Mixing == false) {
                fertilizerTime = millis() - 1;
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
                    Serial.print( food );
                    Serial.println( F(" + Fertilizer...") );
                #endif
            } else if ( tankStatus < 2 && is_Mixing == true && fertilizer_added == false){
                unsigned int fertilizerDosageTime = ( (int)EEPROM.read(EEFeedContainer) / 100 * (int)EEPROM.read(EEFeedFrtlzRate) ) / (int)EEPROM.read(EEFeedFrtlzFlow);
                if( (fertilizerTime != 0) && (millis() - fertilizerTime) > (fertilizerDosageTime * 1000) ) {
                    digitalWrite(FRTL, LOW);
                    digitalWrite(WATR, HIGH);
                    fertilizer_added = true;
                    #ifdef _Serial
                        Serial.print( food );
                        Serial.println( F(" + Diluting Food Solution...") );
                    #endif
                } // -> Dilute with water
            } else if ( tankStatus > 2 && is_Mixing == true){
                #ifdef _Serial
                    Serial.print( food );
                    Serial.println( F(" : Solution Tank [OK].") );
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
            fdt_reset();
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
                warmUpTime = millis() - 1;
            }
        } else if (awake == false && warm == true){
            if( (warmUpTime != 0) && (millis() - warmUpTime) > ((int)EEPROM.read(EEDutyCycleWarmUp) * 1000) ) { awake = true; lastDuty = millis(); }
        } else if (awake == true && warm == true) { // it's warm & awake...
          //if( scheduledGrowLightNow() == true ){ setNextEvent(2); }
            // time to sleep?
            if(ldr_status == 0 && smt_status == 0 && is_irrigating == false && is_supplyingGrowLight == false && is_heating == false && is_cooling == false && is_Fanning == false && is_Fogging == false && is_Mixing == false){
                warm = false; awake = false;
                warmUpTime = 0;
                digitalWrite(SVCC, LOW);
            }
        }
    }
}
