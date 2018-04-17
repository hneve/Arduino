/*################################################
 # Fotoino 
 # Started 4.1.13
 ################################################*/
 



#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);   // Setup LiquidCrystal

/*################################################
# ALL CONSTANTS 
################################################*/
const int LCD_BACKLIGHT    = 10;  // 0 = off - 255 = full on
const int SENSOR           = A1;

const int FLASH_TRIGGER    = 13;
const int FLASH_TRIGGER2   = 12;

const int CAM_PREFOCUS     = 11;  
const int CAM_TRIGGER      = 3;

const int btnRIGHT         = 0;
const int btnUP            = 1;
const int btnDOWN          = 2;
const int btnLEFT          = 3;
const int btnSELECT        = 4;
const int btnNONE          = 5;

const int ModeFlashtrigger  = 1;
const int ModeCameratrigger = 2;
const int ModeTimeLapse     = 3;

/*################################################
# ALL GLOBAL VARIABLES 
################################################*/


boolean        digit_plus         = false;       
boolean        digit_minus        = false;
int            menu_mode          = 0;
int            menu_mode_digit    = 0;
int            time_digit         = 0;
int            toleranse          = 10;
int            level;
int            low_level;
int            high_level;
int            calibrationTime    = 5; //sec
unsigned int   timeDelay          = 0; //ms
int            backlight_val      = 128;
unsigned int   modeShow           = 0;
int            modeSet            = 0;

boolean        draw               = true;
boolean        run                = false;

/*################################################
# SETUP
################################################*/
void setup(){
    pinMode(FLASH_TRIGGER, OUTPUT);
    pinMode(FLASH_TRIGGER2, OUTPUT);
    pinMode(CAM_PREFOCUS, OUTPUT);
    pinMode(CAM_TRIGGER, OUTPUT);
    
    analogWrite(LCD_BACKLIGHT ,backlight_val);
    
    lcd.begin(16, 2);              
    lcd.setCursor(0,0);
    lcd.print("Fotoino V.0.8");  
    lcd.setCursor(0,1);
    lcd.print("Hneve 2013");
    delay(1000);
    lcd.clear();
}

/*################################################
# MAIN loop
################################################*/
void loop(){
  if(modeSet == 0){
    lcd.setCursor(0,0);
    lcd.print("Set Mode:");
    lcd.setCursor(0,1);
    
    if(modeShow == 0) lcd.print("Flashtrigger ");
    if(modeShow == 1) lcd.print("Cameratrigger");
    if(modeShow == 2) lcd.print("TimeLapse    ");
    
    switch (BTN_read()) {
      case btnUP:  { modeShow++; break;}
      case btnDOWN:   { modeShow--; break;}
      case btnSELECT: { modeSet =  modeShow + 1 ; break;}
     }  
   if(modeShow > 2) modeShow = 0;
  }
  
if(modeSet == ModeFlashtrigger)  FlashTrigger();
if(modeSet == ModeCameratrigger) CameraTrigger();
if(modeSet == ModeTimeLapse)     TimeLapse(); 
}

/*################################################
# Button sensor 
################################################*/
int BTN_read(){
 
int adc_key_in = analogRead(0);      // read the value from the sensor  
  if (adc_key_in > 1000) return btnNONE; 
  if (adc_key_in < 50) {  
      while(analogRead(0) < 1000){} 
      return btnRIGHT;  }
  if (adc_key_in < 195)  {
      while(analogRead(0) < 1000){} 
      return btnUP; }
  if (adc_key_in < 380) {  
      while(analogRead(0) < 1000){} 
      return btnDOWN; }
  if (adc_key_in < 555) {
      while(analogRead(0) < 1000){} 
      return btnLEFT; }
  if (adc_key_in < 790) {
      while(analogRead(0) < 1000){} 
      return btnSELECT;   }
  return btnNONE;  // when all others fail, return this...
}

/*################################################
# SENSOR INPUT SELF CALIBRATION
################################################*/
void calibrate(int time){ 
  time *= 1000;
  lcd.noBlink(); 
  lcd.setCursor(0,0);
  lcd.print("Calibrating ");

  int level = analogRead(SENSOR);
  low_level = level;
  high_level = level;
  unsigned long stamp = millis() + time;
  while (millis() < stamp) {
      level = analogRead(SENSOR);
      if (level < low_level) { low_level = level - toleranse;}
      if (level > high_level) { high_level = level + toleranse;}
      lcd.setCursor(13,0);
      lcd.print(1 + (stamp - millis()) / 1000);
    
      lcd.setCursor(0,1);
      //lcd.print(level);
      lcd.print("H");
      lcd.print(high_level);
      lcd.print(" L");
      lcd.print(low_level);  
  }
  if(low_level < 200) low_level = 200;
  lcd.clear();
}


