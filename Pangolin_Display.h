
//https://github.com/adafruit/Adafruit_SSD1306

// https://upload.wikimedia.org/wikipedia/commons/f/f8/Codepage-437.png
/*
#define OLED_CS    48// 13
#define OLED_RESET 46
#define OLED_DC    44// 12  // common 50
#define OLED_CLK   42 //13 // common  52
#define OLED_MOSI  40// 11 //common 51
*/


#define OLED_GND 46// 13
#define OLED_POWER 44// 13


#define OLED_CS    40// 13
#define OLED_RESET 38
#define OLED_DC    36// 12  // common 50
#define OLED_CLK   34 //13 // common  52
#define OLED_MOSI  32// 11 //common 51

/*
#define OLED_CS    A0
#define OLED_RESET A1
#define OLED_DC    A2
#define OLED_CLK   A3
#define OLED_MOSI  A4
*/


/*
#define OLED_GND 45// 13
#define OLED_POWER 43// 13
#define OLED_CS    39// 13
#define OLED_RESET 37
#define OLED_DC    35// 12  // common 50
#define OLED_CLK   33 //13 // common  52
#define OLED_MOSI  31// 11 //common 51
*/


   // pinMode(OLED_POWER, OUTPUT);
    //pinMode(8, OUTPUT);
    //pinMode(9, OUTPUT);
/*
#define OLED_CS    34// 13
#define OLED_RESET 36
#define OLED_DC    38// 12  // common 50
#define OLED_CLK   40 //13 // common  52
#define OLED_MOSI  42// 11 //common 51
*/




#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

  /*
 * In Adafruit_SSD1306.h
 * 1. uncomment #define SSD1306_128_64
2. comment #define SSD1306_128_32
3. comment #define SSD1306_96_16
In the example ssd1306_128x64_i2c
4. add #define SSD1306_LCDHEIGHT 64
 *
 */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SSD1306_LCDHEIGHT 64
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//Adafruit_SSD1306 display(OLED_RESET);
//DISPLAY INITIALIZER
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
void Display_SwitchOff(){
      digitalWrite(OLED_POWER, LOW);       // turn on pullup resistors
      digitalWrite(OLED_GND, LOW);       // keep GND Level   
      Menu = MENU_NULL;       
}

void Display_ReInit(byte Timer){
    pinMode(OLED_GND, OUTPUT); 
    pinMode(OLED_POWER, OUTPUT);
    OLED_Timer = Timer; // 10 sec
    digitalWrite(OLED_POWER, HIGH);       // 
    digitalWrite(OLED_GND, LOW);       //  

    
    if(!display.begin(SSD1306_SWITCHCAPVCC)) {  //    SSD1306_EXTERNALVCC
        Serial.println("SSD1306 allocation failed");
            //for(;;); // Don’t proceed, loop forever
    }
     Serial.println("Dsiplay ReInitilized");  
 //   display.clearDisplay();
 //   display.setTextSize(3);
 //   display.setTextColor(WHITE);  //0  white on black
 //    display.setTextColor(0);  //1     Black on white
    display.dim(1); // lower brightness 
 //   display.setCursor(0, 0);
 //   display.println();
//    display.println("DATALOG"); 
 //   display.display();
}

void DisplayInit(void){
    //-- DISPLAY INIT --//
    pinMode(OLED_GND, OUTPUT);
    digitalWrite(OLED_GND, LOW);       // keep GND Level           
    pinMode(OLED_POWER, OUTPUT);
    OLED_Timer = 40; // 20-> 10 sec
    digitalWrite(OLED_POWER, HIGH);       // turn on pullup resistors
    
    Serial.println("Display Initing");
    if(!display.begin(SSD1306_SWITCHCAPVCC)) {  //  SSD1306_EXTERNALVCC
        Serial.println("SSD1306 allocation failed");
  
     // for(;;); // Don’t proceed, loop forever
      }
       display.dim(1); // lower brightness 
   // display.begin(SSD1306_SWITCHCAPVCC);
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);  //0  white on black
 //    display.setTextColor(0);  //1     Black on white
    display.setCursor(0, 0);
    display.println();
    display.println("DATALOG"); 
      display.display();
  //    delay(250);
     // Adafruit_SSD1306::dim  ( 1 ) //1 == lower brightness // 0 == full brightness
      //display.dim       
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
 // delay(2000);
}

void displayValues(void)
{
  if(DisplayInitDelay == OFF)return;

  //testdrawchar();
  //return;
  
  display.clearDisplay();
  display.setTextSize(1);
  
   display.setCursor(0, 1);
    display.print(Str_Time); 
    display.print(' ');    
    display.println(Str_Date);       
    //Str_Date + Str_Time + ' ';  


      switch(SDCard.Status){
        case 0 :display.print("No Card  Reset 2 Run");// 26
        break;
        case 1 :display.print("SD1 Card ");display.print(SD_Volume);display.print(" Gb ");
        break;
        case 2 :display.print("SD2 Card ");display.print(SD_Volume);display.print(" Gb ");
        break;
        case 3 :display.print("SDHC Card ");display.print(SD_Volume);display.print(" Gb");
        break; 
        default://display.print("Unknown Problem !");   
                  display.print("Card Problem    !");       
     }
     display.println();
    
    
    if(SDCard.Status != SD_NOT_Present){
      if (SDCard.LogStatus){
        display.print("Log2 ");
        display.print(LOG_FILE);  // 8 left
      }
      else display.print("LOG OFF Sample:");//15
      String DispSample="";
      switch(SampleTime){
       case TASK_500MSEC:DispSample = " 500mS";
          break;        
        case TASK_1SEC : DispSample = "  1Sec";
          break; 
        case TASK_2SEC : DispSample = "  2Sec";
          break;        
        case TASK_5SEC : DispSample = "  5Sec";
          break;  
        case TASK_10SEC :DispSample = " 10Sec";
          break; 
        case TASK_20SEC :DispSample = " 20Sec";
          break;            
        case TASK_60SEC :DispSample = " 60Sec";
          break;     
      }  
      display.println(DispSample);       
    }
    else{
      Serial.println("Card Problem");
        display.println("Problem");    
    }
     //4th line
   // DisplayFullSensors();
    DisplayTestDevices();
  
    display.setCursor(0, 57);
    DisplayMenu();

   display.display();
}

String Disp_MENU_NULL = "ENTER  UP DOWN    ESC";
String Disp_MENU1 =     "LOG START & STOP MENU";
String Disp_MENU2 =     "LOG SAMPLE TIME MENU ";
String Disp_MENU3 =     "DISPLAY STANDBYE MENU";

String Disp_MENU1_SUB1= "START LOG? If Yes ENT"; 
String Disp_MENU1_SUB2= "STOP LOG? If Yes ENT ";  
String Disp_MENU3_SUB1= "STNDBY Enb?If Yes ENT"; 
String Disp_MENU3_SUB2= "STNDBY Dis?If Yes ENT";  

String Disp_MENU2_SUB= "Enter -> ";  //9
String Disp_MENU2_SUB1= " 500 mSec   "; //12
String Disp_MENU2_SUB2= " 1 Sec      "; //12
String Disp_MENU2_SUB3= " 2 Sec      "; //12
String Disp_MENU2_SUB4= " 5 Sec      "; //12
String Disp_MENU2_SUB5= " 10 Sec     "; //12
String Disp_MENU2_SUB6= " 20 Sec     "; //12
String Disp_MENU2_SUB7= " 60 Sec     "; //12

void DisplayMenu(void){
  switch(Menu){
    case MENU_NULL : display.print(Disp_MENU_NULL); 
      break;
    case MENU1 :     display.print(Disp_MENU1); 
      break;
    case MENU2 :     display.print(Disp_MENU2); 
      break;
    case MENU3 :     display.print(Disp_MENU3); 
      break;

    case MENU1_SUB1 :display.print(Disp_MENU1_SUB1); 
      break;
    case MENU1_SUB2 :display.print(Disp_MENU1_SUB2); 
      break;

    case MENU2_SUB1 : display.print(Disp_MENU2_SUB); display.print(Disp_MENU2_SUB1);  
      break;
    case MENU2_SUB2 : display.print(Disp_MENU2_SUB); display.print(Disp_MENU2_SUB2);    
      break;
    case MENU2_SUB3 : display.print(Disp_MENU2_SUB); display.print(Disp_MENU2_SUB3);   
      break;
    case MENU2_SUB4 :display.print(Disp_MENU2_SUB); display.print(Disp_MENU2_SUB4);    
      break;
    case MENU2_SUB5 :display.print(Disp_MENU2_SUB); display.print(Disp_MENU2_SUB5);    
      break;
    case MENU2_SUB6 : display.print(Disp_MENU2_SUB); display.print(Disp_MENU2_SUB6);   
      break;
    case MENU2_SUB7 :display.print(Disp_MENU2_SUB); display.print(Disp_MENU2_SUB7);    
      break;

    case MENU3_SUB1 : display.print(Disp_MENU3_SUB1);  
      break;
    case MENU3_SUB2 :  display.print(Disp_MENU3_SUB2); 
      break;

    default: 
    break;
  }
}
void DisplayFullSensors(void){
    display.print("x");
    display.print(Accelometer.x); //6
    
    display.print(" y");
    display.print(Accelometer.y); //7
    

     display.print(" z");
     display.println(Accelometer.z); //7     
    
    
    display.print(Values.TemperatureSi072_Ch1,1);
   //  display.print("°");                     
    display.print("C %");
    display.print(Values.Humidity_Ch1,1); //
    display.print(" Lm:");
    display.println(Values.Luminosity); //

    display.print(Values.Pressure,0);
    display.print("hPA ");
    display.print(Values.TemperatureBMP,1);
   //  display.print("\0x7F");   
    display.print("C ");  
    display.print(Values.Altitude,1);
    display.println("m"); 
}

void DisplayTestDevices(void){
   //  display.print("");
     display.print(Current_Mains_Raw); //7     

     display.print("  ");
     display.print(Current_Mains); //7  
     display.print(" A");

     display.print("  ");
     display.print(Current_MainsAverage); //7  
     display.println(" A");
  // display.print("1:"); 
    if (!isnan(Values.TemperatureSi072_Ch1)) {
          display.print(Values.TemperatureSi072_Ch1,1);                    
          display.write(247);display.print("C"); 
    }
    else  display.print("------");  


   
   if (!isnan(Values.Humidity_Ch1)) {
    display.print(" %");
    display.print((int)Values.Humidity_Ch1); // 
   }
    else  display.print("----");  
    
    display.print(" ");


  // display.print("2:");  
   if (!isnan(Values.TemperatureSi072_Ch2)) {
        display.print(Values.TemperatureSi072_Ch2,1);
        display.write(247);display.print("C"); 
   }
    else  display.print("------");  

    if (!isnan(Values.Humidity_Ch2)) {
    display.print(" %");
    display.print((int)Values.Humidity_Ch2); // 
   }
    else  display.print("----");      

  //  display.print(" ");
 //  display.print("3:");  
      display.println(""); 
     if (!isnan(Values.TemperatureSi072_Ch3)) {
        display.print(Values.TemperatureSi072_Ch3,1);
        display.write(247);display.print("C"); 
   }
    else  display.print("--");  

    if (!isnan(Values.Humidity_Ch3)) {
    display.print(" %");
    display.print((int)Values.Humidity_Ch3); // 
   }
    else  display.print("--");     

    display.print(deBugString); // 

    display.println();

    display.print(EE_Id_EString); // 
               
}
