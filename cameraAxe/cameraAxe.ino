////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010, 2011, 2012)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
// Compiled with Arduino Software 1.0 (http://arduino.cc)

// Select/deselect the features that you want by selecting or deselecting
// the following #defines. Deselecting is done by putting 2 slashes in front of
// the #define like //#define. Not all features can be selected as the program
// becomes too big for the CameraAxe 5 and CameraAxe 5 Shield.  You will get an
// error message if the code is too big to load.
#define USE_ADVANCEDMENU
#define USE_PROJECTILEMENU
#define USE_VALVEMENU
#define USE_INTERVALOMETERMENU
//#define USE_GRAVITYMENU
//#define USE_IRREMOTEMENU
//#define USE_STACKERMENU
//#define USE_PANORAMAMENU
//#define USE_JOGMENU

//
// REVISIONS:
// 400  (10-03-2010) - Initial release (complete rewrite from 3.0 branch)
//
// 401  (11-21-2010) - Added Threshold to general menu
//                   - Added OR and AND of sensors to general menu
//                   - Added HDR stops to intervalometer menu
//                   - Added 10 second backlight mode to settings menu
//                   - Fixed bug in timelapse mode so it is more accurate
//                   - Disabled the internal pullup resistors for Sensor1 and Sensor2 (makes them work much better with some sensors)
//                   - General Menu now will retrigger if it gets a constant trigger signal
//
// 402 (1-29-2011)   - Recoded intervalometer to work on timers rather than delicate polling loops (improved accuracy of timer in this mode)
//                   - Fixed but in intervalometer where shot after delay didn't count towards total shots
//                   - Increased duration between photos in intervalometer to 0.5 seconds since some cameras (Rebel) weren't working reliably with shorter times
//                   - Added a mirror lockup option to intervalometer
//                   - Updated source and display to use the "photo mode" instead of "trigger mode"
//                   - Fix bug where setting LCD backlight off wasn't correctly applied if you cycled the power
//
// 5.0 (7-23-2011)   - Modified code to handle hardware changes in Camera Axe 5.0
//                   - Added define to control flash duration (FLASH_DURATION_MS)
//                   - Changed the name of the "general" menu to the "advanced" to prevent people from thinking general settings apply to other menus
//                   - Add a delay before initializing the LCD to prevent race condition (fixes rare blank lcd bug)
//                   - Improved detectButtonPress to better detect button presses in a more reliable way for the analog button method used
//                   - Added test code for test jig that runs when the activate button is pressed down when turned on
//                   - Removed g_eepromShadow and restructured the eeprom code around this to reduce memory usage
//                   - Various minor cleanups and bug fixes
//
// 5.1 (9-1-2011)    - Added a test Motor Menu
//                   - Cleanup menu code so it's easier to add new menus
//                   - Changed scrolling algorithm in the Advanced Menu
//                   - Changed power off strings from Off_Sen1/Off_Sen2 to Off_Dev1/Off_Dev2.  Code already did this and it is the correct way because it's more flexible (can and/or sensors in a device)
//                   - Added EEPROM_PRESET_GROUP - a way to save and restore different presents
//                   - Modified Advanced menu to work better with bulb time of 0
//                   - Added analog or digital option for sensors in the Advanced menu (replaces fast menu)
//                   - Removed Fast Trigger menu
//                   - Added startup mode control (EEPROM_GENERAL_STARTUP_MODE)
//
// 5.2 (12-9-2011)   - Updated valve to allow triggering two valves independently (Help from Jeffouille and PTB)
//                   - Added Gravity Menu (Help from Snagrom and PTB) (not included in official release)
//                   - Moved test motor menu out of official release
//                   - Added auto increment option to flash delay on valve menu
//                   - Made the analog reads much faster (at expense of accuracy)
//                   - Fixed a bug in intervalometer where larger bulb times wouldn't work because they would overflow the precision of an int16.
//                   - Added more robust button debouncing
//                   - Switched to using Arduino 0023
//
// 5.3 (3-28-2011)   - Added gravity menu back
//                   - Updated LCD lib to work with Arduino 1.0
//                   - Changed file types from *.pde to *.ino as recommended for Arduino 1.0
//                   - Changed bulb time in the Advanced Sensor Menu from 00->99 to 000.0->999.9 seconds.
//                   - Fixed a bug in backlight option that turned backlight off after 10 seconds.  It now turns off backlight in photomode.
//                   - Updated code with USE_* defines to make it easy for people to pick the menus they want
//                   - Added stacker, panorama, jog, and irRemote features (Help from Don)
//                   - Moved all strings to language.ino to make translation easier
//                   - Started using PROGMEM for all strings to reduce sram usage
//                   - Added 3rd drop to Valve 1
//                   - Added eeprom size so defaults are reloaded when new program uploaded
//                   - Menus are now user selectable since there are more memus than will fit in program memory
//                   - Increased max value of "Flash Delay Auto Inc" from 9 ms to 99 ms
//                   - Added seconds between shots to valve menu
//                   - Added number of shots to valve menu
//                   - Fixed bug with intervalometer where minutes and seconds weren't correctly clamping to 59
//                   - Cleanup of how decimal is handled in numbers to greatly simply code
//                   - Increased amount of time for shutter to open in valve menu from 150 ms to 400 ms
//                   - Increased delay times for camera shutter from 20 ms to 400 ms on stacker and panorama
//                   - Added "Number of Repeats" to force multiple shots in valve menu
//                   - Change default drop size in valve menus to 4 since that works better with the new valve sensor
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This code is the firmware for the CameraAxe5 which is a flash/camera trigger tool.  This hardware contains the following
// interfaces to the external world:
// A) 9 buttons to control a menu
// B) 1 LCD to display a menu (DOGM128 128x64)
// C) 2 3.5 mm jack for driving flashes or cameras (called devices in code)
// D) 2 3.5 mm jack to analog sensors (power to these ports can be enabled/disable via a transistor - over 100mA supported)
// E) 2 LEDs to indicate flash/camera triggering
// F) 1 LED to indicate battery charging

#include <EEPROM.h>
#include <DogmCA.h>

PROGMEM const prog_char _tx_releaseButtons[]     = "Release buttons";
PROGMEM const prog_char _tx_resetSuccessful[]    = "   Reset Successful";
PROGMEM const prog_char _tx_removeMenu[]         = "Remove a menu, Reload";
PROGMEM const prog_char _tx_eepromSize[]         = " EEPROM_SIZE=";

uint8_t forcePrototypesHere;  //function prototypes are inserted just before
                              //the first variable or function

// Memory offsets into EEPROM (saves settings when power goes off)
enum {
  EEPROM_ACTIVE_MENU, 
#ifdef USE_ADVANCEDMENU
  EEPROM_ADV_DEVICE_TRIG_SENSOR1,
  EEPROM_ADV_DEVICE_TRIG_SENSOR2,
  EEPROM_ADV_DEVICE_DELAY1,
  EEPROM_ADV_DEVICE_DELAY2,
  EEPROM_ADV_DEVICE_CYCLE1,
  EEPROM_ADV_DEVICE_CYCLE2,
  EEPROM_ADV_DEVICE_PREFOCUS1,
  EEPROM_ADV_DEVICE_PREFOCUS2,
  EEPROM_ADV_SENSOR_LOW_HIGH_THRESH1,
  EEPROM_ADV_SENSOR_LOW_HIGH_THRESH2,
  EEPROM_ADV_SENSOR_TRIG_VAL1,
  EEPROM_ADV_SENSOR_TRIG_VAL2,
  EEPROM_ADV_SENSOR_POWER1,
  EEPROM_ADV_SENSOR_POWER2,
#endif
#ifdef USE_PROJECTILEMENU
  EEPROM_PROJECTILE_SENSOR_DISTANCE,
  EEPROM_PROJECTILE_SENSOR_LOW_HIGH,
  EEPROM_PROJECTILE_INCH_CM,
#endif
#ifdef USE_GRAVITYMENU
  EEPROM_GRAVITY_SENSOR_DISTANCE,
  EEPROM_GRAVITY_INCH_CM,
  EEPROM_GRAVITY_SHUTTER_LAG,
#endif
#ifdef USE_VALVEMENU
  EEPROM_VALVE_V1_DROP1_SIZE,
  EEPROM_VALVE_V1_DROP2_DELAY,
  EEPROM_VALVE_V1_DROP2_SIZE,
  EEPROM_VALVE_V1_DROP3_DELAY,
  EEPROM_VALVE_V1_DROP3_SIZE,
  EEPROM_VALVE_V2_DROP1_SIZE,
  EEPROM_VALVE_V2_DROP2_DELAY,
  EEPROM_VALVE_V2_DROP2_SIZE,
  EEPROM_VALVE_V2_START_OFFSET,
  EEPROM_VALVE_FLASH_DELAY,
  EEPROM_VALVE_FLASH_INCREMENT,
  EEPROM_VALVE_SEC_BETWEEN_SHOTS,
  EEPROM_VALVE_NUM_SHOTS,
  EEPROM_VALVE_REPEAT_SHOTS, 
#endif
#ifdef USE_INTERVALOMETERMENU
  EEPROM_INTERVALOMETER_DELAY_SEC,
  EEPROM_INTERVALOMETER_DELAY_MIN,
  EEPROM_INTERVALOMETER_DELAY_HOUR,
  EEPROM_INTERVALOMETER_SEC,
  EEPROM_INTERVALOMETER_MIN,
  EEPROM_INTERVALOMETER_HOUR,
  EEPROM_INTERVALOMETER_SHOTS,
  EEPROM_INTERVALOMETER_BULB,
  EEPROM_INTERVALOMETER_HDRSTOPS,
  EEPROM_INTERVALOMETER_MIRRORLOCKUP,
#endif
#ifdef USE_IRREMOTEMENU
  EEPROM_DEVICE1_TYPE,
  EEPROM_DEVICE2_TYPE,
#endif
#ifdef USE_STACKERMENU
  EEPROM_STACKERMOTOR_LOCATION,
  EEPROM_STACKERMOTOR_UNITS,
  EEPROM_STACKERMOTOR_SPEED,
  EEPROM_STACKERMOTOR_DIR,
  EEPROM_STACKERMOTOR_TOTPHOTOS,
  EEPROM_STACKERMOTOR_SETTLETIME,
  EEPROM_STACKERMOTOR_STEPSPERUNIT,
  EEPROM_STACKERMOTOR_DISTANCE,
#endif
#ifdef USE_PANORAMAMENU
  EEPROM_PANOMOTOR_LOCATION,
  EEPROM_PANOMOTOR_STEPSPERREV,
  EEPROM_PANOMOTOR_SETTLETIME,
  EEPROM_PANOMOTOR_TOTPHOTOS,
  EEPROM_PANOMOTOR_TOTALDEGREES,
  EEPROM_PANOMOTOR_SPEED,
  EEPROM_PANOMOTOR_DIR,
#endif
#ifdef USE_JOGMENU
  EEPROM_MOTORJOG_LOCATION,
  EEPROM_MOTORJOG_BSTEPSPERJOG,
  EEPROM_MOTORJOG_LSTEPSPERJOG,
  EEPROM_MOTORJOG_STEPSPERSEC,
#endif
  EEPROM_SIZE_PREVIOUS,
  EEPROM_GENERAL_LCD_BACKLIGHT,
  EEPROM_GENERAL_STARTUP_MODE,
  EEPROM_GENERAL_PRESET_GROUP,
  EEPROM_SIZE  // EEPROM_SIZE must be at end of enum
};

// If EEPROM_SIZE ever gets to 127 look at EEPROM_GENERAL_PRESET_GROUP to make sure we don't overflow the 1028 bytes of EEPROM memory (4096 for mega)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define EEPROM_MAXSIZE 511
#else
  #define EEPROM_MAXSIZE 127
#endif

//this is no good but something like this would be desirable
//#if EEPROM_SIZE >= EEPROM_MAXSIZE
//  #error EEPROM_SIZE is >= EEPROM_MAXSIZE
//#endif

//////////Start developer #defines and global variables////////////////////////
//currently none
/////////// End developer #defines and global variables////////////////////////

#ifdef USE_IRREMOTEMENU
enum {WIRED, CANON_TYPE, CANONWLDC100_TYPE, MINOLTA_TYPE, NIKON_TYPE, OLYMPUS_TYPE, PENTAX_TYPE, SONY_TYPE};
void setShutterFunc(int device, int deviceType);
void (*g_shutterNow[2])(int pin);  //camera type for device 1 and 2
#endif

#ifdef USE_STACKERMENU
enum {INCHES, CENTIMETERS};
#endif

// Some older flashes might need flash duration to be set lower (like 1-10)
#define FLASH_DURATION_MS 200

// global enums
enum { BUTTON_NONE, BUTTON_MENU, BUTTON_SELECT, BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_CAM1, BUTTON_CAM2, BUTTON_ACTIVATE};
enum { MENUMODE_MENU, MENUMODE_PHOTO};
enum {
#ifdef USE_ADVANCEDMENU
  MENU_ADVANCED_SENSOR, 
#endif
#ifdef USE_PROJECTILEMENU
  MENU_PROJECTILE, 
#endif
#ifdef USE_GRAVITYMENU
  MENU_GRAVITY, 
#endif
#ifdef USE_VALVEMENU
  MENU_VALVE,
#endif
#ifdef USE_INTERVALOMETERMENU
  MENU_INTERVALOMETER, 
#endif
#ifdef USE_STACKERMENU
  MENU_STACKER,
#endif
#ifdef USE_PANORAMAMENU
  MENU_PANORAMA, 
#endif
#ifdef USE_JOGMENU
  MENU_JOG, 
#endif
////////////////// Start developer menu identifiers ///////////////////////////
#ifdef USE_NEWMENU         // change the ADD_NEW_MENU as appropriate
  MENU_NEW,                // change here and below as appropriate
#endif
/////////////////// End developer menu identifiers ////////////////////////////

MENU_GENERAL_SETTINGS, MENU_MAX_SIZE};  // Active menu

// Global Variables
DogmCA        g_dogm(getLcdPinA0());        // LCD class
volatile byte g_menuMode;                   // Tracks if you are in menu mode or photo mode
byte          g_cursorPos;                  // Cursor's current position while editing a number
boolean       g_editVal;                    // Editing mode or normal mode
byte          g_mPos;                       // Position in the menu
unsigned long g_backlightMs;                // Time until backlight turns off
char          g_cbuf[30];                   // Buffer for characters

#define DEVICE1_SHUTTER_PIN    3
#define DEVICE1_FOCUS_PIN      4

////////////////////////////////////////
// Function: setup - This function gets run once during startup
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void setup()
{
  //Serial.begin(9600); // open hw serial for debugging

  g_menuMode  = MENUMODE_MENU;
  g_cursorPos = 0;
  g_editVal   = false;
  g_mPos      = 0;

  setupCameraAxePins();
  g_dogm.setFont(font_5x8, 5, 8);

  // Holding activate button when turning on the Camera Axe forces a factory reset
  if ((detectButtonPress(false) == BUTTON_ACTIVATE) || 
      (eepromReadInt(EEPROM_SIZE_PREVIOUS) != (int)EEPROM_SIZE))
  {
    loadDefaultSettings();
    testMode();
    buttonDebounce(10);  // 10 ms debounce
    eepromWriteInt(EEPROM_SIZE_PREVIOUS, (int)EEPROM_SIZE);
  }
  
  validateEepromValues();

  if (eepromReadInt(EEPROM_GENERAL_LCD_BACKLIGHT) == 0)
    lcdPower(LOW);
  else
    lcdPower(HIGH);

  if (eepromReadInt(EEPROM_GENERAL_LCD_BACKLIGHT) == 2)
    g_backlightMs = millis() + 10000; // 10 seconds
  else
    g_backlightMs = 0xffffffff;
    
  g_menuMode = eepromReadInt(EEPROM_GENERAL_STARTUP_MODE);

#ifdef USE_IRREMOTEMENU
  setShutterFunc(0, eepromReadInt(EEPROM_DEVICE1_TYPE));
  setShutterFunc(1, eepromReadInt(EEPROM_DEVICE2_TYPE));
#endif
}

////////////////////////////////////////
// Function: loadDefaults - Loads the default values into the eeprom
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void loadDefaultSettings()
{
  int i;
  if(EEPROM_SIZE < EEPROM_MAXSIZE)  //if out of eeprom space, don't reset EEPROM
  { 
    for (i=3; i>=0; --i)
    {
      eepromWriteInt(EEPROM_GENERAL_PRESET_GROUP, i); // Special - Must be first
    
      eepromWriteInt(EEPROM_ACTIVE_MENU, 0);
    
#ifdef USE_ADVANCEDMENU  
      eepromWriteInt(EEPROM_ADV_DEVICE_TRIG_SENSOR1, 1);
      eepromWriteInt(EEPROM_ADV_DEVICE_TRIG_SENSOR2, 0);
      eepromWriteInt(EEPROM_ADV_DEVICE_DELAY1, 0);
      eepromWriteInt(EEPROM_ADV_DEVICE_DELAY2, 0);
      eepromWriteInt(EEPROM_ADV_DEVICE_CYCLE1, 20);
      eepromWriteInt(EEPROM_ADV_DEVICE_CYCLE2, 20);
      eepromWriteInt(EEPROM_ADV_DEVICE_PREFOCUS1, 0);
      eepromWriteInt(EEPROM_ADV_DEVICE_PREFOCUS2, 0);
      eepromWriteInt(EEPROM_ADV_SENSOR_LOW_HIGH_THRESH1, 1);
      eepromWriteInt(EEPROM_ADV_SENSOR_LOW_HIGH_THRESH2, 1);
      eepromWriteInt(EEPROM_ADV_SENSOR_TRIG_VAL1, 700);
      eepromWriteInt(EEPROM_ADV_SENSOR_TRIG_VAL2, 700);
      eepromWriteInt(EEPROM_ADV_SENSOR_POWER1, 0);
      eepromWriteInt(EEPROM_ADV_SENSOR_POWER2, 0);
#endif
#ifdef USE_PROJECTILEMENU
      eepromWriteInt(EEPROM_PROJECTILE_SENSOR_DISTANCE, 60);
      eepromWriteInt(EEPROM_PROJECTILE_SENSOR_LOW_HIGH, 0);
      eepromWriteInt(EEPROM_PROJECTILE_INCH_CM, 0);
#endif
#ifdef USE_GRAVITYMENU
      eepromWriteInt(EEPROM_GRAVITY_SENSOR_DISTANCE, 120);
      eepromWriteInt(EEPROM_GRAVITY_INCH_CM, 0);
      eepromWriteInt(EEPROM_GRAVITY_SHUTTER_LAG, 0);
#endif
#ifdef USE_VALVEMENU
      eepromWriteInt(EEPROM_VALVE_V1_DROP1_SIZE, 4);
      eepromWriteInt(EEPROM_VALVE_V1_DROP2_DELAY, 0);
      eepromWriteInt(EEPROM_VALVE_V1_DROP2_SIZE, 0);
      eepromWriteInt(EEPROM_VALVE_V1_DROP3_DELAY, 0);
      eepromWriteInt(EEPROM_VALVE_V1_DROP3_SIZE, 0);
      eepromWriteInt(EEPROM_VALVE_V2_DROP1_SIZE, 4);
      eepromWriteInt(EEPROM_VALVE_V2_DROP2_DELAY, 0);
      eepromWriteInt(EEPROM_VALVE_V2_DROP2_SIZE, 0);
      eepromWriteInt(EEPROM_VALVE_V2_START_OFFSET, 0);
      eepromWriteInt(EEPROM_VALVE_FLASH_DELAY, 200);
      eepromWriteInt(EEPROM_VALVE_FLASH_INCREMENT, 0);
      eepromWriteInt(EEPROM_VALVE_SEC_BETWEEN_SHOTS, 0);
      eepromWriteInt(EEPROM_VALVE_NUM_SHOTS, 0);
      eepromWriteInt(EEPROM_VALVE_REPEAT_SHOTS, 0);
#endif
#ifdef USE_INTERVALOMETERMENU
      eepromWriteInt(EEPROM_INTERVALOMETER_DELAY_SEC, 0);
      eepromWriteInt(EEPROM_INTERVALOMETER_DELAY_MIN, 0);
      eepromWriteInt(EEPROM_INTERVALOMETER_DELAY_HOUR, 0);
      eepromWriteInt(EEPROM_INTERVALOMETER_SEC, 30);
      eepromWriteInt(EEPROM_INTERVALOMETER_MIN, 0);
      eepromWriteInt(EEPROM_INTERVALOMETER_HOUR, 0);
      eepromWriteInt(EEPROM_INTERVALOMETER_SHOTS, 0);
      eepromWriteInt(EEPROM_INTERVALOMETER_BULB, 10);
      eepromWriteInt(EEPROM_INTERVALOMETER_HDRSTOPS, 0);
      eepromWriteInt(EEPROM_INTERVALOMETER_MIRRORLOCKUP, 0);
#endif
#ifdef USE_IRREMOTEMENU
      eepromWriteInt(EEPROM_DEVICE1_TYPE,        WIRED);
      eepromWriteInt(EEPROM_DEVICE2_TYPE,        WIRED);
#endif
#ifdef USE_STACKERMENU
      eepromWriteInt(EEPROM_STACKERMOTOR_LOCATION,      0);
      eepromWriteInt(EEPROM_STACKERMOTOR_UNITS,    INCHES);
      eepromWriteInt(EEPROM_STACKERMOTOR_SPEED,       100);
      eepromWriteInt(EEPROM_STACKERMOTOR_DIR,           0);
      eepromWriteInt(EEPROM_STACKERMOTOR_TOTPHOTOS,     1);
      eepromWriteInt(EEPROM_STACKERMOTOR_SETTLETIME, 1000);
      eepromWriteInt(EEPROM_STACKERMOTOR_STEPSPERUNIT, 25);
      eepromWriteInt(EEPROM_STACKERMOTOR_DISTANCE,   5000);
#endif
#ifdef USE_PANORAMAMENU
      eepromWriteInt(EEPROM_PANOMOTOR_LOCATION,       0);
      eepromWriteInt(EEPROM_PANOMOTOR_STEPSPERREV,  200);
      eepromWriteInt(EEPROM_PANOMOTOR_SETTLETIME,  1000);
      eepromWriteInt(EEPROM_PANOMOTOR_TOTPHOTOS,      2);
      eepromWriteInt(EEPROM_PANOMOTOR_TOTALDEGREES, 360);
      eepromWriteInt(EEPROM_PANOMOTOR_SPEED,        100);
      eepromWriteInt(EEPROM_PANOMOTOR_DIR,            0);
#endif
#ifdef USE_JOGMENU
      eepromWriteInt(EEPROM_MOTORJOG_LOCATION,        0);
      eepromWriteInt(EEPROM_MOTORJOG_LSTEPSPERJOG,    1);
      eepromWriteInt(EEPROM_MOTORJOG_BSTEPSPERJOG,   10);
      eepromWriteInt(EEPROM_MOTORJOG_STEPSPERSEC,    60);
#endif
/////////////// Start developer eeprom default values /////////////////////////

//////////////// End developer eeprom default values //////////////////////////
      eepromWriteInt(EEPROM_GENERAL_LCD_BACKLIGHT, 1);
      eepromWriteInt(EEPROM_GENERAL_STARTUP_MODE, 0);
    }
  }

  lcdPower(HIGH);
  g_dogm.start();
  do 
  {
    g_dogm.setXY(26, 41);
    g_dogm.print(getStr(_tx_releaseButtons));
    g_dogm.setXY(11, 32);
    if(EEPROM_SIZE < EEPROM_MAXSIZE)
    {
      g_dogm.print(getStr(_tx_resetSuccessful));
    }
    else
    {
      g_dogm.print(getStr(_tx_removeMenu));
    }
    g_dogm.setXY(26, 21);
    g_dogm.print(getStr(_tx_eepromSize));
    g_dogm.print((int)EEPROM_SIZE);
  } 
  while( g_dogm.next() );

  i = HIGH;
  while(EEPROM_SIZE >= EEPROM_MAXSIZE)  //out of eeprom space, blink forever
  {
    delay(500);
    lcdPower(i);
    i = (i==HIGH?LOW:HIGH);
  }
  
  delay(5000);
  lcdPower(LOW);
}

////////////////////////////////////////
// Function: validateEepromValues - Clamps values in eeprom to valid range
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void validateEepromValues()
{
  eepromClamp(EEPROM_ACTIVE_MENU, 0, MENU_MAX_SIZE-1);

#ifdef USE_ADVANCEDMENU  
  eepromClamp(EEPROM_ADV_DEVICE_TRIG_SENSOR1, 0, 4);
  eepromClamp(EEPROM_ADV_DEVICE_TRIG_SENSOR2, 0, 4);
  eepromClamp(EEPROM_ADV_DEVICE_DELAY1, 0, 9999);
  eepromClamp(EEPROM_ADV_DEVICE_DELAY2, 0, 9999);
  eepromClamp(EEPROM_ADV_DEVICE_CYCLE1, 0, 9999);
  eepromClamp(EEPROM_ADV_DEVICE_CYCLE2, 0, 9999);
  eepromClamp(EEPROM_ADV_DEVICE_PREFOCUS1, 0, 1);
  eepromClamp(EEPROM_ADV_DEVICE_PREFOCUS2, 0, 1);
  eepromClamp(EEPROM_ADV_SENSOR_LOW_HIGH_THRESH1, 0, 3);
  eepromClamp(EEPROM_ADV_SENSOR_LOW_HIGH_THRESH2, 0, 3);
  eepromClamp(EEPROM_ADV_SENSOR_TRIG_VAL1, 0, 999);
  eepromClamp(EEPROM_ADV_SENSOR_TRIG_VAL2, 0, 999);
  eepromClamp(EEPROM_ADV_SENSOR_POWER1, 0, 2);
  eepromClamp(EEPROM_ADV_SENSOR_POWER2, 0, 2);
#endif
#ifdef USE_PROJECTILEMENU
  eepromClamp(EEPROM_PROJECTILE_SENSOR_DISTANCE, 0, 999);
  eepromClamp(EEPROM_PROJECTILE_SENSOR_LOW_HIGH, 0, 1);
  eepromClamp(EEPROM_PROJECTILE_INCH_CM, 0, 1);
#endif
#ifdef USE_GRAVITYMENU
  eepromClamp(EEPROM_GRAVITY_SENSOR_DISTANCE, 0, 999);
  eepromClamp(EEPROM_GRAVITY_INCH_CM, 0, 1);
  eepromClamp(EEPROM_GRAVITY_SHUTTER_LAG, 0, 999);
#endif
#ifdef USE_VALVEMENU
  eepromClamp(EEPROM_VALVE_V1_DROP1_SIZE, 0, 999);
  eepromClamp(EEPROM_VALVE_V1_DROP2_DELAY, 0, 999);
  eepromClamp(EEPROM_VALVE_V1_DROP2_SIZE, 0, 999);
  eepromClamp(EEPROM_VALVE_V1_DROP3_DELAY, 0, 999);
  eepromClamp(EEPROM_VALVE_V1_DROP3_SIZE, 0, 999);
  eepromClamp(EEPROM_VALVE_V2_DROP1_SIZE, 0, 999);
  eepromClamp(EEPROM_VALVE_V2_DROP2_DELAY, 0, 999);
  eepromClamp(EEPROM_VALVE_V2_DROP2_SIZE, 0, 999);
  eepromClamp(EEPROM_VALVE_V2_START_OFFSET, 0, 999);
  eepromClamp(EEPROM_VALVE_FLASH_DELAY, 0, 999);
  eepromClamp(EEPROM_VALVE_FLASH_INCREMENT, 0, 99);
  eepromClamp(EEPROM_VALVE_SEC_BETWEEN_SHOTS, 0, 99);
  eepromClamp(EEPROM_VALVE_NUM_SHOTS, 0, 999);
  eepromClamp(EEPROM_VALVE_REPEAT_SHOTS, 0, 9);
#endif
#ifdef USE_INTERVALOMETERMENU
  eepromClamp(EEPROM_INTERVALOMETER_DELAY_SEC, 0, 59);
  eepromClamp(EEPROM_INTERVALOMETER_DELAY_MIN, 0, 59);
  eepromClamp(EEPROM_INTERVALOMETER_DELAY_HOUR, 0, 99);
  eepromClamp(EEPROM_INTERVALOMETER_SEC, 0, 59);
  eepromClamp(EEPROM_INTERVALOMETER_MIN, 0, 59);
  eepromClamp(EEPROM_INTERVALOMETER_HOUR, 0, 99);
  eepromClamp(EEPROM_INTERVALOMETER_SHOTS, 0, 9999);
  eepromClamp(EEPROM_INTERVALOMETER_BULB, 0, 9999);
  eepromClamp(EEPROM_INTERVALOMETER_HDRSTOPS, 0, 9);
  eepromClamp(EEPROM_INTERVALOMETER_MIRRORLOCKUP, 0, 1);
#endif
#ifdef USE_IRREMOTEMENU
  eepromClamp(EEPROM_DEVICE1_TYPE,      WIRED, SONY_TYPE);
  eepromClamp(EEPROM_DEVICE2_TYPE,      WIRED, SONY_TYPE);
#endif
#ifdef USE_STACKERMENU
  eepromClamp(EEPROM_STACKERMOTOR_LOCATION, 0, 1);
  eepromClamp(EEPROM_STACKERMOTOR_UNITS, INCHES, CENTIMETERS);
  eepromClamp(EEPROM_STACKERMOTOR_SPEED, 1,1000);
  eepromClamp(EEPROM_STACKERMOTOR_DIR, 0,1);
  eepromClamp(EEPROM_STACKERMOTOR_TOTPHOTOS, 1,100);
  eepromClamp(EEPROM_STACKERMOTOR_SETTLETIME, 0,10000);
  eepromClamp(EEPROM_STACKERMOTOR_STEPSPERUNIT, 1,9999);
  if(eepromReadInt(EEPROM_STACKERMOTOR_UNITS) == INCHES)
    eepromClamp(EEPROM_STACKERMOTOR_DISTANCE, 1, 9999); //9.999 inches
  else
    eepromClamp(EEPROM_STACKERMOTOR_DISTANCE, 1, 2599); //25.99 cm
#endif
#ifdef USE_PANORAMAMENU
  eepromClamp(EEPROM_PANOMOTOR_LOCATION, 0, 1);
  eepromClamp(EEPROM_PANOMOTOR_STEPSPERREV, 1,9999);
  eepromClamp(EEPROM_PANOMOTOR_SETTLETIME, 1,10000);
  eepromClamp(EEPROM_PANOMOTOR_TOTPHOTOS, 1,600);
  eepromClamp(EEPROM_PANOMOTOR_TOTALDEGREES, 0,360);
  eepromClamp(EEPROM_PANOMOTOR_SPEED, 1,1000);
  eepromClamp(EEPROM_PANOMOTOR_DIR, 0,1);
#endif
#ifdef USE_JOGMENU
  eepromClamp(EEPROM_MOTORJOG_LOCATION,       0, 1);
  eepromClamp(EEPROM_MOTORJOG_BSTEPSPERJOG,    1, 200);
  eepromClamp(EEPROM_MOTORJOG_LSTEPSPERJOG,    1, 200);
  eepromClamp(EEPROM_MOTORJOG_STEPSPERSEC,    0,1000);
#endif

/////////////// Start developer eeprom validate values ////////////////////////

//////////////// End developer eeprom validate values /////////////////////////

  eepromClamp(EEPROM_GENERAL_LCD_BACKLIGHT, 0, 2);
  eepromClamp(EEPROM_GENERAL_STARTUP_MODE, 0, 1);
  eepromClamp(EEPROM_GENERAL_PRESET_GROUP, 0, 3);

}

////////////////////////////////////////
// Function: loop - This function gets run in a continous loop
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void loop()
{
  g_dogm.start();
  
  if (g_menuMode == MENUMODE_MENU)
  {
    setAnalogReadSpeed(0);

    do 
    {
      if (eepromReadInt(EEPROM_GENERAL_LCD_BACKLIGHT) == 2)
      {
        if (millis() > g_backlightMs)
          lcdPower(LOW);
        else
          lcdPower(HIGH);
      }
      
      setDefaultPins(); // Sets all pins back to default values

      // Menus
      switch (eepromReadInt(EEPROM_ACTIVE_MENU))
      {
#ifdef USE_ADVANCEDMENU
        case MENU_ADVANCED_SENSOR:
          advancedSensorMenu();
          break;
#endif
#ifdef USE_PROJECTILEMENU
        case MENU_PROJECTILE:
          projectileMenu();
          break;
#endif
#ifdef USE_GRAVITYMENU          
        case MENU_GRAVITY:
          gravityMenu();
          break;
#endif
#ifdef USE_VALVEMENU
        case MENU_VALVE:
          valveMenu();
          break;
#endif
#ifdef USE_INTERVALOMETERMENU
        case MENU_INTERVALOMETER:
          intervalometerMenu();
          break;
#endif
#ifdef USE_STACKERMENU
	case MENU_STACKER:
          stackerMenu();
          break;
#endif
#ifdef USE_PANORAMAMENU
	case MENU_PANORAMA:
          panoMenu();
          break;
#endif
#ifdef USE_JOGMENU
	case MENU_JOG:
          jogMenu();
          break;
#endif
//////////////////// Start developer menu mode calls //////////////////////////

///////////////////// End developer menu mode calls ///////////////////////////
        case MENU_GENERAL_SETTINGS:
          generalSettingsMenu();
          break;
      }
    } while( g_dogm.next() );
  }
  
  if (g_menuMode == MENUMODE_PHOTO)
  {
    if ((g_menuMode == MENUMODE_PHOTO) && (eepromReadInt(EEPROM_GENERAL_LCD_BACKLIGHT) == 2))
      lcdPower(LOW);

    setAnalogReadSpeed(1);

    // Photo Mode Functions
    switch (eepromReadInt(EEPROM_ACTIVE_MENU))
    {
#ifdef USE_ADVANCEDMENU
      case MENU_ADVANCED_SENSOR:
        advancedSensorFunc();
        break;
#endif
#ifdef USE_PROJECTILEMENU
      case MENU_PROJECTILE:
        projectileFunc();
        break;
#endif
#ifdef USE_GRAVITYMENU          
      case MENU_GRAVITY:
        gravityFunc();
        break;
#endif
#ifdef USE_VALVEMENU
      case MENU_VALVE:
        valveFunc();
        break;
#endif
#ifdef USE_INTERVALOMETERMENU
      case MENU_INTERVALOMETER:
        intervalometerFunc();
        break;
#endif
#ifdef USE_STACKERMENU
	case MENU_STACKER:
          stackerFunc();
          break;
#endif
#ifdef USE_PANORAMAMENU
	case MENU_PANORAMA:
          panoFunc();
          break;
#endif
#ifdef USE_JOGMENU
	case MENU_JOG:
          jogFunc();
          break;
#endif
//////////////////// Start developer photo mode calls /////////////////////////

///////////////////// End developer photo mode calls //////////////////////////
      case MENU_GENERAL_SETTINGS:
        // No photo mode for the settings menu
        g_menuMode = MENUMODE_MENU;
        break;
    }
  }
}


