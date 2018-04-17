
void FlashTrigger () {
lcd.noBlink();
lcd.setCursor(0,0);
lcd.print("FlashTrigger:");  
printtimeDelay();
printsensorvalue();

switch (BTN_read()) {
      case btnRIGHT:  { if(menu_mode_digit != 5)menu_mode_digit++; break;}
      case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
      case btnSELECT: { flashTriggerRun(); break;}
      }

while (menu_mode_digit > 0) {
    if(timeDelay > 9999) timeDelay = 9999;  // Max 9999 ms
    if(toleranse > 99) toleranse = 99;
    if(toleranse == 0) toleranse =  1;
  
    switch (menu_mode_digit) {
        case 1: { // delay digit 1
              int inc = 1000;
              lcd.setCursor(2,1);
              lcd.blink();
              switch (BTN_read()) {
                  case btnRIGHT:  { menu_mode_digit++; break;}
                  case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
                  case btnUP:     { timeDelay += inc; printtimeDelay(); break;}
                  case btnDOWN:   { timeDelay -= inc; printtimeDelay(); break;}
                  case btnSELECT: { flashTriggerRun(); break;}
                  }
              break; 
              }
              
        case 2: {
              int inc = 100;
              lcd.setCursor(3,1);
              lcd.blink();
              switch (BTN_read()) {
                  case btnRIGHT:  { menu_mode_digit++; break;}
                  case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
                  case btnUP:     { timeDelay += inc; printtimeDelay(); break;}
                  case btnDOWN:   { timeDelay -= inc; printtimeDelay(); break;}
                  case btnSELECT: { flashTriggerRun(); break;}
                }
              break; 
              }
        case 3: {
              int inc = 10;
              lcd.setCursor(4,1);
              lcd.blink();
              switch (BTN_read()) {
                  case btnRIGHT:  { menu_mode_digit++; break;}
                  case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
                  case btnUP:     { timeDelay += inc; printtimeDelay(); break;}
                  case btnDOWN:   { timeDelay -= inc; printtimeDelay(); break;}
                  case btnSELECT: { flashTriggerRun(); break;}
                }
              break; 
              }
      
      case 4: {
              int inc = 1;
              lcd.setCursor(5,1);
              lcd.blink();
              switch (BTN_read()) {
                  case btnRIGHT:  { menu_mode_digit++; break;}
                  case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
                  case btnUP:     { timeDelay += inc; printtimeDelay(); break;}
                  case btnDOWN:   { timeDelay -= inc; printtimeDelay(); break;}
                  case btnSELECT: { flashTriggerRun(); break;}
                }
              break; 
              }
              
      case 5: { // setting toleranse
              int inc = 1;
              lcd.setCursor(12,1);
              lcd.blink();
              switch (BTN_read()) {
                  case btnRIGHT:  { menu_mode_digit = 0; break;}
                  case btnLEFT:   { if(menu_mode_digit != 0)menu_mode_digit--; break;}
                  case btnUP:     { toleranse += inc; printtimeDelay(); break;}
                  case btnDOWN:   { toleranse -= inc; printtimeDelay(); break;}
                  case btnSELECT: { flashTriggerRun(); break;}
                }
              break; 
              }
   
}}}

/*################################################
# RUN FLASH TRIGGER
################################################*/
void flashTriggerRun(){ 
lcd.clear();

 // ADC in turbo mode 
 bitClear(ADCSRA,ADPS0) ;
 bitClear(ADCSRA,ADPS1) ;
 bitSet(ADCSRA,ADPS2) ;
      
calibrate(5);
analogWrite(LCD_BACKLIGHT, 0);          // Turn off backlight LCD
digitalWrite(CAM_TRIGGER, HIGH);        // camera open


    level = analogRead(SENSOR);


    while ((level > low_level) && (level < high_level)) { level = analogRead(SENSOR); }

    delay(timeDelay);                      // Trigger delay


bitSet(PORTB, 5);  // FAST digitalWrite(FLASH_TRIGGER, HIGH);
digitalWrite(FLASH_TRIGGER2, HIGH);
delay(10);  
digitalWrite(FLASH_TRIGGER, LOW);
digitalWrite(FLASH_TRIGGER2, LOW);
digitalWrite(CAM_TRIGGER, LOW);
analogWrite(LCD_BACKLIGHT, backlight_val);
// Turn OFF turbo mode !!
bitSet(ADCSRA,ADPS0) ;
bitSet(ADCSRA,ADPS1) ;
bitSet(ADCSRA,ADPS2) ;

menu_mode_digit = 0; //reset menu
}

void printtimeDelay(){
  lcd.setCursor(0,1);
  lcd.print("D:"); 
  if (timeDelay > 9999) timeDelay = 9999;
  if (timeDelay < 1000) lcd.print(" ");
  if (timeDelay < 100) lcd.print(" ");
  if (timeDelay < 10) lcd.print(" "); 
  lcd.print(timeDelay); 
  lcd.print("ms");
  lcd.print(" T:");
  lcd.print(toleranse);
  lcd.print(" ");
}

void printsensorvalue(){
  lcd.setCursor(13,0);
  //lcd.print("");
  int x = analogRead(SENSOR);
  lcd.print(constrain(x, 1, 999));
  
}
