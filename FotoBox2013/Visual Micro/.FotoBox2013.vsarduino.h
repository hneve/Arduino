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
void get_buttons();
void update_lcd();
void set_mode();
void sensor_setup();
void set_delay(int nr);
void RUN();
void calibrate(int time);
void RUN_analog_flashtrigger();
void RUN_digital_flashtrigger();
void RUN_analog_cameratrigger();
void RUN_digital_cameratrigger();
void RUN_timelapse();
void PrintRunning();
void SystemTest();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Dropbox\Workspace\Arduino\FotoBox2013\FotoBox2013.ino"
