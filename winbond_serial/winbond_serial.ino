/*
 * Copyright (c) 2013 by Felix Rusu <felix@lowpowerlab.com>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

// This sketch is an example of using the SPIFlash library with a Moteino
// that has an onboard SPI Flash chip. This sketch listens to a few serial commands
// Hence type the following commands to interact with the SPI flash memory array:
// - 'd' dumps the first 256bytes of the flash chip to screen
// - 'e' erases the entire memory chip
// - 'i' print manufacturer/device ID
// - [0-9] writes a random byte to addresses [0-9] (either 0xAA or 0xBB)
// Get the SPIFlash library from here: https://github.com/LowPowerLab/SPIFlash

#include <SPIFlash.h>
#include <RFM12B.h>
#include <SPI.h>
#include <avr/wdt.h>

#define SERIAL_BAUD      115200
char input = 0;
long lastPeriod = -1;

//////////////////////////////////////////
// flash(SPI_CS, MANUFACTURER_ID)
// SPI_CS          - CS pin attached to SPI flash chip (8 in case of Moteino)
// MANUFACTURER_ID - OPTIONAL, 0x1F44 for adesto(ex atmel) 4mbit flash
//                             0xEF30 for windbond 4mbit flash
//////////////////////////////////////////
SPIFlash flash(8, 0xEF40);

void setup(){
  Serial.begin(SERIAL_BAUD);
  Serial.print("Start...");

  if (flash.initialize())
    Serial.println("Init OK!");
  else
    Serial.println("Init FAIL!");
}

void loop(){
  // Handle serial input (to allow basic DEBUGGING of FLASH chip)
  // ie: display first 256 bytes in FLASH, erase chip, write bytes at first 10 positions, etc
  if (Serial.available() > 0) {
    input = Serial.peek();
    if (input == 'd') //d=dump flash area
    {
      Serial.println("Flash content:");
      int counter = 0;
	  unsigned long countTo = Serial.parseInt() - 1;
	  int fold = 0;
	  unsigned long	lineNumber = 0;
	  Serial.print(lineNumber);
	  Serial.print(": ");
	  Serial.write(0x09);

      while(counter<=countTo){
        Serial.print(flash.readByte(counter++), HEX);
		if(fold++ == 7) {
			lineNumber += 8;
			Serial.println();
			Serial.print(lineNumber);
			Serial.print(":");
			Serial.write(0x09);
			fold = 0;
		}
		else {
			//Serial.print(" : ");
			Serial.write(0x09);
		}
		
      }
      
      Serial.println();
    }
    else if (input == 'e')
    {
      Serial.print("Erasing Flash chip ... ");
      flash.chipErase();
      while(flash.busy());
      Serial.println("DONE");
    }
    else if (input == 'i')
    {
      Serial.print("DeviceID: ");
      Serial.println(flash.readDeviceId(), HEX);
    }
    else if (input >= 48 && input <= 57) //0-9
    {
      Serial.print("\nWriteByte("); Serial.print(input); Serial.print(")");
      flash.writeByte(input-48, millis()%2 ? 0xaa : 0xbb);
    }
	else if (input == 'w'){  // WHAT,TIMES,FROM
		int thebyte = Serial.parseInt();
		unsigned int howMany = Serial.parseInt() - 1;
		unsigned int from = Serial.parseInt();
		unsigned int counter = from;
		howMany += counter;
		while(counter <= howMany){
			flash.writeByte(counter++,thebyte);
		}
		Serial.print(howMany - from + 1);
		Serial.println(" bytes written");
	}
	else if (input == 'F'){
		long memoryLimit = 2097151;
		long counter = 0;
		long startTime = millis();
		Serial.println("Fill started!");
		while(counter <= memoryLimit){
			flash.writeByte(counter, counter);
			counter++;
		}
		long runTime = millis() - startTime;
		Serial.print(runTime);
		Serial.println("ms");
		Serial.println("Ok!");
	}
	else if (input == 'r') //d=dump flash area
	{
		//Serial.println("RAW Flash content:");
		long counter = 0;
		long countTo = 1024;// 2097151;
		int flip=0;
		while(counter<=countTo){
			//Serial.print(flash.readByte(counter),HEX);
			Serial.write(flash.readByte(counter));
			//Serial.print(" ");
			counter++;
			if(flip == 64){
				Serial.println();
				flip = 0;
			}
			//flip++;
		}
	}
	
    input = Serial.read();
  }

  // Periodically blink the onboard LED while listening for serial commands
  if ((int)(millis()/500) > lastPeriod)
  {
    lastPeriod++;
    pinMode(9, OUTPUT);
    digitalWrite(9, lastPeriod%2);
  }
}