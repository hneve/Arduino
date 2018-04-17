/* 
	Editor: http://www.visualmicro.com
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Generic ESP8266 Module, Platform=esp8266, Package=esp8266
*/

#define __ESP8266_ESp8266__
#define __ESP8266_ESP8266__
#define _VMDEBUG 1
#define ARDUINO 106012
#define ARDUINO_MAIN
#define F_CPU 80000000L
#define printf iprintf
#define __ESP8266__
#define __ets__
#define ICACHE_FLASH
#define F_CPU 80000000L
#define LWIP_OPEN_SRC
#define ARDUINO 106012
#define ARDUINO_ESP8266_ESP01
#define ARDUINO_ARCH_ESP8266
//
//
void callback(char* topic, byte* payload, unsigned int length);
void bell(int bell);
void chime1();
void chime2();
void chime3();
void chime4();
void chime5();
void chime6();

#include "common.h" 
#include "pins_arduino.h" 
#include "Arduino.h"
#include "esp_blink.ino"
