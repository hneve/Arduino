#include <SPI.h>
const int clkCS = 9;

void transmitRtc(const byte reg, const byte data){
  digitalWrite(clkCS, LOW);
  SPI.transfer (reg);
  SPI.transfer (data);
  digitalWrite(clkCS, HIGH);
} // end of transmitRtc

int RTC_init(){
  //Setup RTC communication.
  pinMode(clkCS,OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3); // both mode 1 & 3 should work
  //set control register
  transmitRtc(0x8E,0x60); //60= disable Oscillator and Battery SQ wave @1hz, temp compensation, Alarms disabled
  delay(10);
}

void setup() {
  // put your setup code here, to run once:
  RTC_init();
  transmitRtc(0x85,0b00000010);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
