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
void calibrate(int time);
void mode_sensor_info();
void mode_flash_trigger();
void mode_camera_trigger();
void mode_time_lapse();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Dropbox\Workspace\Arduino\Fotoino\Fotoino.ino"
#include "D:\Dropbox\Workspace\Arduino\Fotoino\CameraTrigger.ino"
#include "D:\Dropbox\Workspace\Arduino\Fotoino\Flashtrigger.ino"
#include "D:\Dropbox\Workspace\Arduino\Fotoino\TimeLapse.ino"