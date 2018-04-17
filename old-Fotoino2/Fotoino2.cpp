// Do not remove the include below
#include "Fotoino2.h"
/*################################################
 # Fotoino SW V.0.1
 # Started 4.1.13
 ################################################*/

/*
SETUP
 */
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/*################################################
# ALL CONSTANTS
################################################*/
const int LCD_BACKLIGHT  = 10;  // 0 = off - 255 = full on
const int SENSOR         = "A1";
const int LASER          = A2;
const int FLASH_TRIGGER  = 2;
const int AMB_LIGHT      = 3;
const int CAM_PREFOCUS   = 11;  // Most likely never to be used.
const int CAM_TRIGGER    = 12;

const int btnRIGHT       = 0;
const int btnUP          = 1;
const int btnDOWN        = 2;
const int btnLEFT        = 3;
const int btnSELECT      = 4;
const int btnNONE        = 5;

const int M_Start_trig     =0;
const int M_Delay          =1;
const int M_Run            =2;
const int MaxMenu          =5;

/*################################################
# ALL GLOBAL VARIABLES
################################################*/

boolean  isCalibrated     = true;
boolean  digit_plus       = false;
boolean  digit_minus      = false;
int      menu_mode        = 0;
int      menu_mode_digit  = 0;
int      time_digit       = 0;
int      toleranse        = 10;
int      level;
int      low_level;
int      high_level;
int      calibrationTime   = 5; //sec
unsigned int trig_delay        = 150; //ms
int      backlight_val     = 128;
/*################################################
# SETUP
################################################*/
void setup(){
pinMode(LASER, OUTPUT);
pinMode(FLASH_TRIGGER, OUTPUT);
pinMode(AMB_LIGHT, OUTPUT);
pinMode(CAM_PREFOCUS, OUTPUT);
pinMode(CAM_TRIGGER, OUTPUT);

analogWrite(LCD_BACKLIGHT ,backlight_val);

lcd.begin(16, 2);
lcd.setCursor(0,0);
lcd.print("Fotoino V.0.3");
lcd.setCursor(0,1);
lcd.print("c Hans Neve 2013");
delay(1000);
lcd.clear();

}


/*################################################
# MAIN loop
################################################*/
void loop(){
lcd.noBlink();
lcd.setCursor(0,0);
lcd.print(F("FT:"));

printDelay();
printsensorvalue();

switch (BTN_read()) {
      case btnRIGHT:  { if(menu_mode_digit != 5)menu_mode_digit++; break;}
      case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
      case btnSELECT: { flashTriggerRun(); break;}
}

while (menu_mode_digit > 0) {
  if(trig_delay > 9999) trig_delay = 9999;  // failsafe
  if(toleranse > 99) toleranse = 99;
  if(toleranse == 0) toleranse =  1;

  switch (menu_mode_digit) {
    case 1: {
          int inc = 1000;
          lcd.setCursor(5,0);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { if(menu_mode_digit != MaxMenu)menu_mode_digit++; break;}
              case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
              case btnUP:     { trig_delay += inc; printDelay(); break;}
              case btnDOWN:   { trig_delay -= inc; printDelay(); break;}
              case btnSELECT: { flashTriggerRun(); break;}
            }
          break; }

    case 2: {
          int inc = 100;
          lcd.setCursor(6,0);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { if(menu_mode_digit != MaxMenu)menu_mode_digit++; break;}
              case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
              case btnUP:     { trig_delay += inc; printDelay(); break;}
              case btnDOWN:   { trig_delay -= inc; printDelay(); break;}
              case btnSELECT: { flashTriggerRun(); break;}
            }
          break; }
    case 3: {
          int inc = 10;
          lcd.setCursor(7,0);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { if(menu_mode_digit != MaxMenu)menu_mode_digit++; break;}
              case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
              case btnUP:     { trig_delay += inc; printDelay(); break;}
              case btnDOWN:   { trig_delay -= inc; printDelay(); break;}
              case btnSELECT: { flashTriggerRun(); break;}
            }
          break; }

  case 4: {
          int inc = 1;
          lcd.setCursor(8,0);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { if(menu_mode_digit != MaxMenu)menu_mode_digit++; break;}
              case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
              case btnUP:     { trig_delay += inc; printDelay(); break;}
              case btnDOWN:   { trig_delay -= inc; printDelay(); break;}
              case btnSELECT: { flashTriggerRun(); break;}
            }
          break; }

  case 5: { // setting toleranse
          int inc = 1;
          lcd.setCursor(15,0);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { if(menu_mode_digit != MaxMenu)menu_mode_digit++; break;}
              case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
              case btnUP:     { toleranse += inc; printDelay(); break;}
              case btnDOWN:   { toleranse -= inc; printDelay(); break;}
              case btnSELECT: { flashTriggerRun(); break;}
            }
          break; }

}}}




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
# SENSORINPUT SELF CALIBRATION
################################################*/
void calibrate(int time){
 lcd.noBlink();
  time *= 1000;
  lcd.setCursor(0,0);
  lcd.print(F("Calibrating "));
  int level = analogRead(SENSOR);
  low_level = level;
  high_level = level;
  unsigned long stamp = millis() + time;
  while (millis() < stamp) {
      level = analogRead(SENSOR);
      if (level < low_level) { low_level = level - toleranse;}
      if (level > high_level) { high_level = level + toleranse;}
      lcd.setCursor(13,0);
      lcd.print((stamp - millis()) / 1000);

      lcd.setCursor(0,1);
      //lcd.print(level);
      lcd.print("H");
      lcd.print(high_level);
      lcd.print(" L");
      lcd.print(low_level);
  }
lcd.clear();
isCalibrated = true;
}

/*################################################
# RUN FLASH TRIGGER
################################################*/
void flashTriggerRun(){
lcd.clear();
calibrate(5);
analogWrite(LCD_BACKLIGHT, 0);
digitalWrite(CAM_TRIGGER, HIGH);

level = analogRead(SENSOR);
while ((level > low_level) && (level < high_level)) {
    level = analogRead(SENSOR);
    }
delay(trig_delay);
digitalWrite(FLASH_TRIGGER, HIGH);
delay(100);
digitalWrite(FLASH_TRIGGER, LOW);
digitalWrite(CAM_TRIGGER, LOW);
analogWrite(LCD_BACKLIGHT, backlight_val);
menu_mode_digit = 0; //reset menu
}




void printDelay(){
  lcd.setCursor(3,0);
  lcd.print("D:");

  if (trig_delay > 9999) trig_delay = 9999;

  if (trig_delay < 1000) lcd.print(" ");
  if (trig_delay < 100) lcd.print(" ");
  if (trig_delay < 10) lcd.print(" ");
  lcd.print(trig_delay);
  lcd.print("ms");
  lcd.print(" T:");
  lcd.print(toleranse);
  lcd.print(" ");
}

void printsensorvalue(){
  lcd.setCursor(0,1);
  lcd.print("S:");
  int x = analogRead(SENSOR);
  lcd.print(constrain(x, 1, 999));
  lcd.print("   ");
}
