void ShowSerialCode(){
    /*
    SerialCode = EEPROM.read(4);
    SerialCode <<= 8;
    SerialCode += EEPROM.read(5);
    Serial.print("SerialCode:");
    Serial.println(SerialCode);
    */

    char c;
    EE_Id_EString =""; 

    Serial.print("EE: ");
    c = (char)EEPROM.read(4);
    EE_Id_EString += String(c);
    Serial.print(c);
    c = (char)EEPROM.read(5);
    EE_Id_EString += String(c);
    Serial.print(c);
    c = (char)EEPROM.read(6);
    EE_Id_EString += String(c);
    Serial.print(c);    
    c = (char)EEPROM.read(7);
    EE_Id_EString += String(c);
    Serial.print(c);

    Serial.print(" EE_Id_EString: ");
    Serial.print(EE_Id_EString);
  
 /*   
    Serial.print(EEPROM.read(4));
    Serial.print(EEPROM.read(5));
    Serial.print(EEPROM.read(6));
    Serial.print(EEPROM.read(7));

 */   
    Serial.println(); 
 }

void MainLoop(void){
  if(LoopTask_250msec){
    LoopTask_250msec = OFF;
    
    // One time after wake up form sleep
    if(OLED_Init == ON){
       #ifndef DEBUG_SIMULATOR_MODE
      Display_ReInit(20);
       #endif
      OLED_Init = OFF;
    }
    if(OLED_Timer) {
       #ifndef DEBUG_SIMULATOR_MODE
      displayValues();
       #endif
    }
    else {
       #ifndef DEBUG_SIMULATOR_MODE
      Display_SwitchOff();
      #endif
    }
      #ifdef LEM_CURRENT_EXISTS        
        CurrentRead();
      #endif 
    
  }
  if(LoopTask_500msec){
    LoopTask_500msec = OFF;
    if(SampleTime == TASK_500MSEC) SD_CardLogTask();
  }
  
  if(LoopTask_1Sec){
    LoopTask_1Sec = OFF;
    #ifndef DEBUG_SIMULATOR_MODE
      RTC_SerialAdj();
      RTC_TimeClock();
    
      Serial.print("Str_Time:");
      Serial.println(Str_Time);
      Serial.print("Str_Date:");
      Serial.println(Str_Date);


      #ifdef WIND_SENSOR_EXISTS   
        WindSensorRead();
      #endif   
  
      #ifdef TEMP_HUM_1_SENSOR_EXISTS
        deBugString = "TmpHmSn1_1";  
        SensorRead_Si072(SI072_FIRST_SENSOR); // MULTIPLEXER NO
        deBugString = "TmpHmSn1_2";
      #endif
      #ifdef TEMP_HUM_2_SENSOR_EXISTS
        deBugString = "TmpHmSn2_1";
        SensorRead_Si072(SI072_SECOND_SENSOR); // MULTIPLEXER NO
        deBugString = "TmpHmSn2_2";
      #endif
      #ifdef TEMP_HUM_3_SENSOR_EXISTS
        deBugString = "TmpHmSn3_1";      
        SensorRead_Si072(SI072_THIRD_SENSOR); // MULTIPLEXER NO
        deBugString = "TmpHmSn3_2";        
      #endif


      
      #ifdef  BAR_PRES_SENSOR_EXISTS
        SensorAlt_Read();
      #endif
      #ifdef LIGHT_SENSOR_EXISTS
        SensorLight_Read();
      #endif
      #ifdef ACCL_GYRO_SENSOR_EXISTS
        SensorAcccel_GyroRead();
      #endif
    #endif // end of  #ifndef DEBUG_SIMULATOR_MODE

    KeyTimeOutCheck();

    if(SampleTime == TASK_1SEC) SD_CardLogTask();
    ShowSerialCode();
    
  }
  if(LoopTask_2Sec){
    LoopTask_2Sec = OFF;
    if(SampleTime == TASK_2SEC) SD_CardLogTask();

 
    
  }
  if(LoopTask_5Sec){
    LoopTask_5Sec = OFF;
    if(SampleTime == TASK_5SEC) SD_CardLogTask();

    SDS_DustSensor();

    DisplayValueTimer++;
    if (DisplayValueTimer > 3)DisplayValueTimer = 0;
    
  }
  if(LoopTask_10Sec){
    LoopTask_10Sec = OFF;
    if(SampleTime == TASK_10SEC) SD_CardLogTask();
    
  }
  if(LoopTask_20Sec){
    LoopTask_20Sec = OFF;
    if(SampleTime == TASK_20SEC) SD_CardLogTask();
    
  }
  if(LoopTask_60Sec){
    LoopTask_60Sec = OFF;
    if(SampleTime == TASK_60SEC) SD_CardLogTask();
    
  }  
}

void CurrentRead(){
  // https://www.onlinegdb.com/edit/Hkmlxi_08
      
    deBugString = "Cur_tRd_1";
     #ifdef ARDUINO_MEGA
    ADCSRA |= (1 << ADEN); // enable adc
         #endif
    delay(1);
    Current_Mains_Raw = analogRead(4);
  //  Current_Mains_Raw_Trim = Current_Mains_Raw;
     //  delay(1);
       delay(1);   
     Mains_Volt_Raw =  analogRead(1);

  //  239vac -> 4.94vdc
  // 242  vac  5vdc  242/1024 = 0.2363281

    Mains_Volt =   (unsigned int)((float)Mains_Volt_Raw * 0.2363281);     

    CurrentArray[CurrentIndexer]= Current_Mains_Raw;
    CurrentIndexer++;
    if(CurrentIndexer >=10){
      CurrentIndexer = 0;
      unsigned int Cumulative = 0;
      for(unsigned int i=0; i<10; i++){
          Cumulative += CurrentArray[i];   
      } 

     
     
     #define Volt_Coeff 4.8828125 // rms 5v
     #define PeakToRms  1.4142135 
    // #define Rms_Coeff  6.90533966  // Volt_Coeff *  PeakToRms    
     #define Rms_Coeff  4.8828125  // Volt_Coeff *  PeakToRms       
   /*
       Current_MainsAverage = ((Cumulative%10) * Volt_Coeff )/10000; 
      // then the decimal part 
      Current_MainsAverage += ((float)(Cumulative/10) * Volt_Coeff )/1000;
*/
       Current_Mains_Rms = ((Cumulative%10) * Rms_Coeff )/10000; 
      // then the decimal part 
      Current_Mains_Rms += ((float)(Cumulative/10) * Rms_Coeff )/1000;
    
   //   Current_Mains_Rms = Current_MainsAverage * 1.414213;  
   //   Current_Mains_Av = Current_Mains_Rms / 1,11;    
    //  Current_MainsAverage = ((float)Cumulative * 5.044)/1000; // 2K2 // 4.7K // 2K2 
     //   Current_MainsAverage = ((float)Cumulative * 9.766)/1000;  //direct
    }

  //  Current_Mains = ((float)Current_Mains_Raw * Rms_Coeff)/1000;
   // Current_Mains *= PeakToRms;
  
 //   Current_Mains = ((float)Current_Mains_Raw * 5.044)/1000;
  //   Current_Mains = ((float)Current_Mains_Raw * 9.766)/1000; // direct no voltage divider

   

   Serial.print("Current Adc: ");  
   Serial.println(Current_Mains_Raw);
   Serial.print("Current Average as Rms(A): ");  
   Serial.println(Current_Mains_Rms);
   Serial.print("Voltage Adc: ");  
   Serial.println(Mains_Volt_Raw);
   Serial.print("Voltage(Vac) :");  
   Serial.println(Mains_Volt);

  
   #ifdef ARDUINO_MEGA
   ADCSRA &= ~ (1 << ADEN);            // turn off ADC
   #endif
      deBugString = "Cur_tRd_2"; 
}

void SDS_DustSensor(void){
      #ifdef PM25_DUST_SENSOR_EXISTS 
         PmResult pm = sds.queryPm();
        if (pm.isOk()) {
              Values.PM25 = pm.pm25;
              Values.PM10 = pm.pm10;
               if(Values.PM25 >= 250.0)Values.PM25 = 250.0;
               if(Values.PM10 >= 250.0)Values.PM25 = 250.0;
               
              
            Serial.print("PM2.5 = ");
             Serial.print(Values.PM25);
             Serial.print(", PM10 = ");       
            Serial.println(Values.PM10);

          // if you want to just print the measured values, you can use toString() method as well
        //  Serial.println(pm.toString());
        } else {
          Serial.print("Could not read values from sensor, reason: ");
          Serial.println(pm.statusToString());
        }
      #endif 

}

void WindSensorRead(){
  /*
     #ifdef ARDUINO_MEGA
    ADCSRA |= (1 << ADEN); // enable adc
         #endif
    delay(1);
  //  Values.WindRaw = analogRead(2);delay(1);
    Values.WindMPH = analogRead(OUT_PINOUT);delay(1);
    Values.WindTemp = analogRead(TMP_PINOUT);
    WindSpeed_Calculation(); 
   #ifdef ARDUINO_MEGA
   ADCSRA &= ~ (1 << ADEN);            // turn off ADC
   #endif
   */
}


void KeyTimeOutCheck(void){
        if(KeyBoardTimeOutEnb == ON){
          if(KeyBoardTimeOut)KeyBoardTimeOut--;
          else {
            KeyBoardTimeOutEnb = OFF;
             Menu =  MENU_NULL;
          }       
        }
}    


void KeyTimeOutStart(void){
    KeyBoardTimeOutEnb = ON;
     KeyBoardTimeOut  = 24;// 24 sec
}



void DisplayWakeUp(void){
      if((OLED_Timer==0) && (OLED_Init == OFF)){
        OLED_Init = ON;
      }//else if(OLED_Timer)OLED_Timer += 10; // add additional time every time any key released
}

void Key_Functions(void){
       if(!KeyLeft_Rel && !KeyMid_Rel && !KeyRight_Rel){
          KeyReleased = ON;      
       }           
          if(KeyReleased && !digitalRead(KEY_LEFT)){
            KeyLeft_Rel = ON;
              #ifdef DEBUG_KEY
              KeyLogger = 1;
              #endif 
            return;
          }
          if(KeyLeft_Rel  &&  digitalRead(KEY_LEFT)){
            KeyLeft_Rel = OFF;
                #ifdef DEBUG_KEY
                KeyLogger = 2;
                #endif  
             DisplayWakeUp();
             EnterMenuKey(); 
             KeyTimeOutStart();        
            return;                   
          }
          if(KeyReleased && !digitalRead(KEY_MID)){
            KeyMid_Rel = ON;
              #ifdef DEBUG_KEY
              KeyLogger = 3;
              #endif            
            return;
          }
          if(KeyMid_Rel  &&  digitalRead(KEY_MID)){
             KeyMid_Rel = OFF;
              #ifdef DEBUG_KEY
              KeyLogger = 4;
              #endif
            DisplayWakeUp();  
            DownMenuKey();
            KeyTimeOutStart();                               
            return;                            
          }
          if(KeyReleased && !digitalRead(KEY_RIGHT)){
            KeyRight_Rel = ON;
              #ifdef DEBUG_KEY
              KeyLogger = 5;
              #endif            
            return;
          }
          if(KeyRight_Rel  &&  digitalRead(KEY_RIGHT)){
            KeyRight_Rel = OFF;
              #ifdef DEBUG_KEY
              KeyLogger = 6;
              #endif
            DisplayWakeUp(); 
           EscMenuKey();
           KeyTimeOutStart();                           
            return;
            
        }
}


void DispEnable(bool Enable){
  if(Enable==ON) {
    DisplaySleepEnable = ON; //go sleep
    OLED_Timer = 20;
  }
  else   DisplaySleepEnable = OFF;    // no sleep
}

void LogEnable(bool Enable){
  /*
  if(Enable == ON)LogPause = ON;
  else{
    LogPause = OFF;SD_KartStop = OFF;// once to get sd kart info only
  }
  */
}


 void  DispExtTimeout(void){
    if(OLED_Timer <= KEYDISP_TIMER) OLED_Timer = KEYDISP_TIMER;
 }

void DownMenuKey(void){
  if(OLED_Timer==0) return;
  DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU_NULL;
      break;
    case MENU1 : Menu = MENU3; //
      break;
    case MENU2 : Menu = MENU1;
      break;
    case MENU3 : Menu = MENU2;
      break;


    case MENU1_SUB1 :  Menu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : Menu =  MENU1_SUB1;//
      break;

    case MENU2_SUB1 :  Menu =  MENU2_SUB7;//
      break;
    case MENU2_SUB2 :  Menu =  MENU2_SUB1;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2_SUB2;//
      break;
    case MENU2_SUB4 :  Menu =  MENU2_SUB3;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2_SUB4;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2_SUB5;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2_SUB6;//
      break;
 //   case MENU2_SUB8 :  Menu =  MENU2_SUB7;//
      break;

    case MENU3_SUB1 :  Menu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3_SUB1;//
      break;


    default: Menu = MENU_NULL;

  }
}

void UpMenuKey(void){
  if(OLED_Timer==0) return;
    DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU_NULL;
      break;
    case MENU1 : Menu = MENU2; //
      break;
    case MENU2 : Menu = MENU3;
      break;
    case MENU3 : Menu = MENU1;
      break;


    case MENU1_SUB1 :  Menu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : Menu =  MENU1_SUB1;//
      break;

    case MENU2_SUB1 :  Menu =  MENU2_SUB2;//
      break;
    case MENU2_SUB2 :  Menu =  MENU2_SUB3;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2_SUB4;//
      break;
    case MENU2_SUB4 :  Menu =  MENU2_SUB5;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2_SUB6;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2_SUB7;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2_SUB1;//
      break;
  //  case MENU2_SUB8 :  Menu =  MENU2_SUB1;//
      break;

    case MENU3_SUB1 :  Menu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3_SUB1;//
      break;


    default: Menu = MENU_NULL;

  }
}


void EscMenuKey(void){
  if(OLED_Timer==0) return;
   DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU_NULL;
      break;
    case MENU1 : Menu = MENU_NULL; //
      break;
    case MENU2 : Menu = MENU_NULL;
      break;
    case MENU3 : Menu = MENU_NULL;
      break;


    case MENU1_SUB1 :  Menu =  MENU1;//
      break;
    case MENU1_SUB2 : Menu =  MENU1;//
      break;

    case MENU2_SUB1 :  Menu =  MENU2;//
      break;
    case MENU2_SUB2 : Menu =  MENU2;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2;//
      break;
    case MENU2_SUB4 : Menu =  MENU2;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2;//
      break;
   // case MENU2_SUB8 :  Menu =  MENU2;//
      break;

    case MENU3_SUB1 :  Menu =  MENU3;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3;//
      break;


    default: Menu = MENU_NULL;

  }
}



void EnterMenuKey(void){
  if(OLED_Timer==0) return;
    DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU1;
  
      break;
    case MENU1 : //Menu = MENU1MIN; // go to sub menu  // sd kart log on
        if(SDCard.LogStatus == ON) Menu = MENU1_SUB2; //already logging
        else  Menu =MENU1_SUB1;
      break;
    case MENU2 : // Menu = MENU2MIN; // call the right menu according to current one 
      switch(SampleTime){
       case TASK_500MSEC: Menu = MENU2_SUB1;
          break;        
        case TASK_1SEC : Menu = MENU2_SUB2;
          break; 
        case TASK_2SEC : Menu = MENU2_SUB3;
          break;        
        case TASK_5SEC : Menu = MENU2_SUB4;
          break;  
        case TASK_10SEC :Menu = MENU2_SUB5;
          break; 
        case TASK_20SEC :Menu = MENU2_SUB6;
          break;            
        case TASK_60SEC :Menu = MENU2_SUB7;
          break; 
        default:
         break;    
      }
      break;
    case MENU3 : // Menu = MENU3MIN;

        if(DisplaySleepEnable == ON) Menu = MENU3_SUB2; //already logging
        else  Menu = MENU3_SUB1;
      break;


    case MENU1_SUB1 :  LogEnable(ON);SDCard.LogStatus = ON;EESetResetLog(ON);
          Menu =  MENU_NULL;//MENU1
      break;
    case MENU1_SUB2 : LogEnable(OFF); SDCard.LogStatus = OFF;EESetResetLog(OFF);// default
          Menu =  MENU_NULL;//MENU1
      break;
    case MENU2_SUB1 :  SampleTime = TASK_500MSEC ;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB2 : SampleTime = TASK_1SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB3 :  SampleTime = TASK_2SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB4 : SampleTime = TASK_5SEC; // default
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB5 :  SampleTime = TASK_10SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB6 :  SampleTime = TASK_20SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB7 :  SampleTime = TASK_60SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU3_SUB1 :  DispEnable(ON);
          Menu =  MENU_NULL;//MENU3
      break;
    case MENU3_SUB2 :  DispEnable(OFF);
          Menu =  MENU_NULL;//MENU3
      break;


    default: Menu = MENU_NULL;

  }
}
void EE_SerNoWrite2_EE(unsigned int SerialNo){
        //  EEPROM.write(Adr, byte);
        // byte t = 0XFFFF & (SerialNo>>8);
    byte t;
    t = (byte)SerialNo;    
    EEPROM.write(4, t);// low byte
    t = (byte)(SerialNo>>8);  
    EEPROM.write(5, t);// high byte
 
}

void EESetResetLog(bool Mode){
     if(Mode == OFF)EEPROM.write(ADDRES_LOG, OFF);// OFF
     else EEPROM.write(ADDRES_LOG, SampleTime);// ON
}

void EEReadLog(void){
    byte Mode = EEPROM.read(ADDRES_LOG);// OFF
     switch(Mode){
       case TASK_500MSEC: SDCard.LogStatus = ON;SampleTime =  Mode; 
          break;        
        case TASK_1SEC : SDCard.LogStatus = ON;SampleTime =  Mode; 
          break; 
        case TASK_2SEC : SDCard.LogStatus = ON;SampleTime =  Mode; 
          break;        
        case TASK_5SEC : SDCard.LogStatus = ON;SampleTime =  Mode; 
          break;  
        case TASK_10SEC :SDCard.LogStatus = ON;SampleTime =  Mode; 
          break; 
        case TASK_20SEC :SDCard.LogStatus = ON;SampleTime =  Mode; 
          break;            
        case TASK_60SEC :SDCard.LogStatus = ON;SampleTime =  Mode; 
          break; 
        default:SDCard.LogStatus = OFF;SampleTime =  TASK_2SEC;  
         break;    
      }   
}
