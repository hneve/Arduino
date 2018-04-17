/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
	Hardware: Arduino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328P__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
int RTC_init();
String readRTC();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\Nivo\OneDrive\Arduino\Dot_clock\Dot_clock.ino"
