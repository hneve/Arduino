#include <SPI.h>

#define dispCS 9

void setup() {
  pinMode(dispCS,OUTPUT);
  digitalWrite(dispCS,HIGH);
  
SPI.begin();
SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
digitalWrite(dispCS,LOW);
SPI.transfer(0x09);//decode mode all
SPI.transfer(0xff);
digitalWrite(dispCS,HIGH);
digitalWrite(dispCS,LOW);
SPI.transfer(0x0b);//scan limit
SPI.transfer(0x07);
digitalWrite(dispCS,HIGH);
digitalWrite(dispCS,LOW);
SPI.transfer(0x0a);//intensity
SPI.transfer(0xff);
digitalWrite(dispCS,HIGH);
digitalWrite(dispCS,LOW);
SPI.transfer(0x0c);//shutdown mode
SPI.transfer(0x01);
digitalWrite(dispCS,HIGH);
}
void loop() {

digitalWrite(dispCS,LOW);
SPI.transfer(0x01);
SPI.transfer(0x08);
digitalWrite(dispCS,HIGH);
delay(500);
digitalWrite(dispCS,LOW);
SPI.transfer(0x01);
SPI.transfer(0x01);
digitalWrite(dispCS,HIGH);
delay(500);
}
