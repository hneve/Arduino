void CameraTrigger()  {
if(run) {  
  lcd.clear();
  calibrate(5);
  analogWrite(LCD_BACKLIGHT, 0);                      // Turn off backlight LCD
  digitalWrite(CAM_PREFOCUS, HIGH);                   // Prefocus

  level = analogRead(SENSOR);
  while ((level > low_level) && (level < high_level)) { level = analogRead(SENSOR); }
 
  delay(timeDelay);
  digitalWrite(CAM_TRIGGER, HIGH);
  delay(40);
  digitalWrite(CAM_TRIGGER, LOW);
  digitalWrite(CAM_PREFOCUS, LOW);
  analogWrite(LCD_BACKLIGHT, backlight_val);
  draw = true;
  run = false;
}  

if(draw){
  lcd.noBlink();
  lcd.setCursor(0,0);
  lcd.print("CameraTrigger:");
  lcd.setCursor(0,1);
  lcd.print("D:"); 
  if (timeDelay > 9999) timeDelay = 9999;
  if (timeDelay < 1000) lcd.print(" ");
  if (timeDelay < 100) lcd.print(" ");
  if (timeDelay < 10) lcd.print(" "); 
  lcd.print(timeDelay); 
  lcd.print("ms T:");
  lcd.print(toleranse);
  lcd.print(" ");
  draw = false;
}
  

  
switch (menu_mode_digit) {
  case 0: { 
          lcd.noBlink();
          switch (BTN_read()) {
              case btnRIGHT:  { menu_mode_digit++; break;}
              case btnSELECT: { run = true;        break;}
              }
           break;}
  
  case 1: { // delay digit 1000
          int inc = 1000;
          lcd.setCursor(2,1);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { menu_mode_digit++; break;}
              case btnLEFT:   { menu_mode_digit--; break;}
              case btnUP:     { timeDelay += inc; draw = true; break;}
              case btnDOWN:   { timeDelay -= inc; draw = true; break;}
              case btnSELECT: { menu_mode_digit = 0; break;}
              }
          break;}
          
  case 2: { // delay digit 100
          int inc = 100;
          lcd.setCursor(3,1);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { menu_mode_digit++; break;}
              case btnLEFT:   { menu_mode_digit--; break;}
              case btnUP:     { timeDelay += inc; draw = true; break;}
              case btnDOWN:   { timeDelay -= inc; draw = true; break;}
              case btnSELECT: { menu_mode_digit = 0; break;}
              }
          break;}
          
  case 3: { // delay digit 10
          int inc = 10;
          lcd.setCursor(4,1);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { menu_mode_digit++; break;}
              case btnLEFT:   { menu_mode_digit--; break;}
              case btnUP:     { timeDelay += inc; draw = true; break;}
              case btnDOWN:   { timeDelay -= inc; draw = true; break;}
              case btnSELECT: { menu_mode_digit = 0; break;}
              }
          break;} 
          
  case 4: { // delay digit 1
          int inc = 1;
          lcd.setCursor(5,1);
          lcd.blink();
          switch (BTN_read()) {
              case btnRIGHT:  { menu_mode_digit++; break;}
              case btnLEFT:   { menu_mode_digit--; break;}
              case btnUP:     { timeDelay += inc; draw = true; break;}
              case btnDOWN:   { timeDelay -= inc; draw = true; break;}
              case btnSELECT: { menu_mode_digit = 0; break;}
              }
          break;}
          
  case 5: { // toleranse
          int inc = 10;
          lcd.setCursor(11,1);
          lcd.blink();
          switch (BTN_read()) {
              //case btnRIGHT:  { menu_mode_digit++; break;}
              case btnLEFT:   { menu_mode_digit--; break;}
              case btnUP:     { if(toleranse != 200)toleranse += inc; draw = true; break;}
              case btnDOWN:   { if(toleranse != 0) toleranse -= inc; draw = true; break;}
              case btnSELECT: { menu_mode_digit = 0; break;}
              }
          break;}
  } 
}
