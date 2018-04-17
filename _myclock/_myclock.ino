#include <SPI.h>
const int  cs=3; //chip select 
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



void setup() {
  Serial.begin(9600);
  RTC_init();
  //day(1-31), month(1-12), year(0-99), hour(0-23), minute(0-59), second(0-59)
  //SetTimeDate(11,12,13,14,15,16); 
  lcd.begin(16,2);
  pinMode(10, OUTPUT);
  analogWrite(10,75);
  ana
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print(ReadTime());
  lcd.setCursor(3,1);
  lcd.print(ReadDate());
  //lcd.print(get_seconds());
  //Serial.println(ReadTimeDate());
  delay(1000);
}



