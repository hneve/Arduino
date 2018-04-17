/*
     Use the I2C bus with EEPROM 24LC64
     Sketch:    eeprom.pde

     Author: hkhijhe
     Date: 01/10/2010


*/

#include <Wire.h> //I2C library



void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  //Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, length);
  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
}




void setup()
{
  Wire.begin(); // initialise the connection
  Wire.setClock(400000L);
  Serial.begin(115200);
  //Serial.println(F("Dump is below this line!"));
  //Serial.println(F("#####################################################################"));
}

void loop()
{
  uint8_t b;
  uint8_t addr = 0; //first address
  uint8_t devBaseAdress = 0x50;

  for (uint8_t device = 0; device < 8; device++) {
    for (uint8_t page = 0; page < 16 ; page++ ) {
      uint8_t adrOffset = 16 * page;
      for (uint8_t p_adress = 0; p_adress < 16; p_adress++) {
        b = i2c_eeprom_read_byte(devBaseAdress + device, p_adress + adrOffset);
       // Serial.print(devBaseAdress + device, HEX);
        //Serial.print(" : ");
        //Serial.print(p_adress + adrOffset, HEX);
        //Serial.print(" : ");
        if (b < 0x10) Serial.print("0");
        Serial.print(b,HEX);
        //Serial.print(", ");
      }
    }
  }
  Serial.println();
  //Serial.println(F("#####################################################################"));
  //Serial.println(F("The End!"));
  while (1);

}
