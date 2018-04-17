/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino/Genuino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 165
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 165
#define ARDUINO_AVR_UNO
#define ARDUINO_ARCH_AVR
extern "C" void __cxa_pure_virtual() {;}

//
void loadDefaultSettings();
void validateEepromValues();
//
void wait(unsigned int time);
void high(unsigned int time, int freq, int pinLED);
void setShutterFunc(int device, int camType);
void sendFocusSignal(int device, int pin, int value);
void sendShutterSignal(int device, int pin, int value);
void CanonShutterNow(int pin);
void CanonWLDC100ShutterNow(int pin);
void MinoltaShutterNow(int pin);
void NikonShutterNow(int pin);
void OlympusShutterNow(int pin);
void PentaxShutterNow(int pin);
void SonyShutterNow(int pin);
void activeButtonISR();
void buttonDebounce(int t);
byte getLcdPinA0();
int readSensorAnalog(byte s);
int readSensorDigital(byte s);
byte readSensorDigitalFast(byte s);
void setSensorPinModes(byte sensor, byte mid, byte tip);
void setAnalogReadSpeed(byte x);
void setSensorPins(byte s, byte mid, byte tip);
void setSensorPowerPin(byte s, byte pow);
void lcdPower(byte val);
void setDevicePins(byte device, byte focus, byte shutter);
void testMode();
void setupCameraAxePins();
void setDefaultPins();
byte lcdSetString(byte x, byte y, boolean selected, boolean edit, byte numStrings, byte stringLength, byte eepromPos, const char *strings);
byte lcdSetNumber(byte x, byte y, boolean selected, boolean edit, byte *cursorPos, byte eepromPos, byte maxDigit, byte numCharsDec, byte numCharsFrac);
void lcdPrintZeros(byte x, byte y, unsigned int bcdVal, byte numDigitsDec, byte numDigitsFrac);
byte detectButtonPress(boolean waitForRelease);
unsigned int decimalToBcd(unsigned int dec);
unsigned int bcdToDecimal(unsigned int bcd);
void eepromWriteInt(int addr, int val);
int eepromReadInt(int addr);
void eepromClamp(int addr, int minVal, int maxVal);
void menuProcessButton(byte button);
unsigned long nanoSec();
void resetTimer0();
void startNanoSec();
void endNanoSec();
void printMenuTitle(const char *titleStr, byte maxChars, byte yAdj);
void printMenuName(const char *nameStr, byte row, byte yAdj);
byte printMenuSetNumber(byte maxChars, byte row, boolean selected, byte eepromPos, byte numCharsDec, byte numCharsFrac, byte maxDigit, byte yAdj);
byte printMenuSetString(byte maxChars, byte row,boolean selected, byte eepromPos, byte numChars, byte numStrings, const char *strings, byte yAdj);
void menuScroll(byte button, byte items);
void stepMotor(byte motorNum, byte stepPos);
void runStackerMotor(int whichMotor, int mSpeed, int mDir, int mSteps);
char *getStr(const char* inStr);
void advancedSensorMenu();
void advancedSensorFunc();
void helpPrintCurSensorValue(byte x, byte y, byte sensor, int lowHighThresh, int *valuesA, int *valuesB);
boolean testDeviceForTrigger(byte s, byte d, boolean deviceOr, boolean deviceAnd, int *sensorVals, int *prevSensorVals,                              int *eepromDeviceTrigSensors, int *eepromSensorLowHighThreshs, int *eepromSensorTrigVals);
void helpSensorDevicePass(unsigned long curTime, byte s, byte d, unsigned long *deviceDelayTimes, unsigned long *deviceCycleTimes, int *sensorVals, int *prevSensorVals,                                         int *eepromDeviceTrigSensors, int *eepromSensorLowHighThreshs, int *eepromSensorTrigVals, int *eepromDeviceDelays,                                          int *eepromSensorPowers, int *eepromDeviceCycles, int *eepromDevicePrefocuses);
void projectileMenu();
void projectileFunc();
void helpProjectileLCD(byte inchCm, unsigned long int inchCmPerSec, boolean sensorFailure);
void valveMenu();
void valveFunc();
void intervalometerMenu();
void helpDrawIntervalometer(int delayHours, int delayMinutes, int delaySeconds, int hours, int minutes, int seconds, int shots, int bulb, int hdrStops);
void intervalometerFunc();
void gravityMenu();
void gravityFunc();
void helpGravityLCD(byte inchCm, unsigned long int startingInchCm, unsigned long int sensorTime, unsigned long int fallTime, boolean sensorFailure);
void stackerMenu();
void helpPrintStacker(int photoCnt, int totalPhotos);
void stackerFunc();
void panoMenu();
void helpPrintPano(int photoCnt, int totalPhotos);
void panoFunc();
void jogMenu();
void helpPrintJog(int dir, int stepsPerJog, int steps, boolean waitForRelease);
void jogFunc();
void generalSettingsMenu();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\arduino.h"
#include <cameraAxe.ino>
#include <IRRemote_helperFuncs.ino>
#include <cameraAxe_helperFuncs.ino>
#include <language.ino>
#include <menu00_advanced.ino>
#include <menu01_projectile.ino>
#include <menu02_valve.ino>
#include <menu03_intervalometer.ino>
#include <menu04_gravitymenu.ino>
#include <menu05_stacker.ino>
#include <menu06_panorama.ino>
#include <menu07_motorjog.ino>
#include <menu99_settings.ino>
