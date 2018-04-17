////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010, 2011, 2012)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <avr/pgmspace.h>

// There are also a few string in cameraAxe.ino that need updating

PROGMEM const prog_char _tx_99_version[]                 = "v5.3";
PROGMEM const prog_char _tx_99_generalSettings[]         = "General Settings       ";
PROGMEM const prog_char _tx_99_backlight[]               = "Backlight";
PROGMEM const prog_char _tx_99_startupMode[]             = "Startup Mode";
PROGMEM const prog_char _tx_99_presetGroup[]             = "Preset Group";

#ifdef USE_IRREMOTEMENU
PROGMEM const prog_char _tx_99_camera1Type[]             = "Camera 1 Type";
PROGMEM const prog_char _tx_99_camera2Type[]             = "Camera 2 Type";
#endif
PROGMEM const prog_char _tx_99_backlightList[]           = "Off   |On    |10 sec";
PROGMEM const prog_char _tx_99_startupList[]             = "Menu |Photo";
PROGMEM const prog_char _tx_99_presetList[]              = "Preset_1|Preset_2|Preset_3|Preset_4";
#ifdef USE_IRREMOTEMENU
PROGMEM const prog_char _tx_99_deviceTypeList[]          = "Wired   |Canon   |Canon100|Minolta |Nikon   |Olympus |Pentax  |Sony    ";
#endif

#ifdef USE_ADVANCEDMENU
PROGMEM const prog_char _tx_00_advancedSensorMenu[]      = "Advanced Sensor Menu";
PROGMEM const prog_char _tx_00_device1Device2[]          = "         Device1  Device2";
PROGMEM const prog_char _tx_00_trigSen[]                 = "Trig Sen";
PROGMEM const prog_char _tx_00_delayms[]                 = "Delay ms";
PROGMEM const prog_char _tx_00_bulbSec[]                 = "Bulb Sec";
PROGMEM const prog_char _tx_00_prefocus[]                = "Prefocus";
PROGMEM const prog_char _tx_00_sensor1Sensor2[]          = "         Sensor1  Sensor2";
PROGMEM const prog_char _tx_00_trigType[]                = "Trig Typ";
PROGMEM const prog_char _tx_00_trigVal[]                 = "Trig Val";
PROGMEM const prog_char _tx_00_power[]                   = "Power";
PROGMEM const prog_char _tx_00_advancedPhotoMode[]       = "Advanced Photo Mode";
PROGMEM const prog_char _tx_00_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const prog_char _tx_00_hi[]                      = "Hi ";
PROGMEM const prog_char _tx_00_low[]                     = "Low";
PROGMEM const prog_char _tx_00_noYesList[]               = "No |Yes";
PROGMEM const prog_char _tx_00_sensorList[]              = "None    |Sensor1 |Sensor2 |S1_or_S2|S1_and_2";
PROGMEM const prog_char _tx_00_trigTypeList[]            = "Low     |High    |Threshld|Digital";
PROGMEM const prog_char _tx_00_lowHighList[]             = "Low|Hi ";
PROGMEM const prog_char _tx_00_powerList[]               = "On      |Off_Dev1|Off_Dev2";
PROGMEM const prog_char _tx_00_camFlashList[]            = "Normal   |Pre-focus";
#endif

#ifdef USE_PROJECTILEMENU
PROGMEM const prog_char _tx_01_projectileMenu[]          = "Projectile Menu";
PROGMEM const prog_char _tx_01_distance[]                = "Distance";
PROGMEM const prog_char _tx_01_lowHighTrigger[]          = "Low/High Trigger";
PROGMEM const prog_char _tx_01_distanceUnits[]           = "Distance Units";
PROGMEM const prog_char _tx_01_projectilePhotoMode[]     = "Projectile Photo Mode";
PROGMEM const prog_char _tx_01_sensor2DidNotTrigger[]    = " Sensor2 did not trigger ";
PROGMEM const prog_char _tx_01_readyForProjectile[]      = "  Ready for projectile!  ";
PROGMEM const prog_char _tx_01_feet[]                    = "Feet";
PROGMEM const prog_char _tx_01_inches[]                  = "Inches";
PROGMEM const prog_char _tx_01_meters[]                  = "Meters";
PROGMEM const prog_char _tx_01_centimeters[]             = "Centimeters";
PROGMEM const prog_char _tx_01_sec[]                     = "/sec: ";
PROGMEM const prog_char _tx_01_pressActivateToExit[]     = "Press Activate to Exit";
PROGMEM const prog_char _tx_01_lowHighList[]             = "Low |High";
PROGMEM const prog_char _tx_01_inchCmList[]              = "Inch|Cm  ";
#endif

#ifdef USE_VALVEMENU
PROGMEM const prog_char _tx_02_valveMenu[]               = "Valve Menu";
PROGMEM const prog_char _tx_02_v1Drop1Size[]             = "V1 Drop1 Size";
PROGMEM const prog_char _tx_02_v1Drop2Delay[]            = "V1 Drop2 Delay";
PROGMEM const prog_char _tx_02_v1Drop2Size[]             = "V1 Drop2 Size";
PROGMEM const prog_char _tx_02_v1Drop3Delay[]            = "V1 Drop3 Delay";
PROGMEM const prog_char _tx_02_v1Drop3Size[]             = "V1 Drop3 Size";
PROGMEM const prog_char _tx_02_flashDelay[]              = "Flash Delay";
PROGMEM const prog_char _tx_02_flashDelayAutoInc[]       = "Flash Delay Auto Inc";
PROGMEM const prog_char _tx_02_secBetwnShots[]           = "Sec Between Shots";
PROGMEM const prog_char _tx_02_numShots[]                = "Number of Shots";
PROGMEM const prog_char _tx_02_repeatShots[]             = "Number of Repeats";
PROGMEM const prog_char _tx_02_v2StartOffset[]           = "V2 Start Offset";
PROGMEM const prog_char _tx_02_v2Drop1Size[]             = "V2 Drop1 Size";
PROGMEM const prog_char _tx_02_v2Drop2Delay[]            = "V2 Drop2 Delay";
PROGMEM const prog_char _tx_02_v2Drop2Size[]             = "V2 Drop2 Size";
PROGMEM const prog_char _tx_02_valvePhotoMode[]          = "Valve Photo Mode";
PROGMEM const prog_char _tx_02_valveActive[]             = "       Valve Active";
#endif

#ifdef USE_INTERVALOMETERMENU
PROGMEM const prog_char _tx_03_intervalometerMenu[]      = "Intervalometer Menu";
PROGMEM const prog_char _tx_03_startDelay[]              = "Start Delay";
PROGMEM const prog_char _tx_03_interval[]                = "Interval";
PROGMEM const prog_char _tx_03_shots[]                   = "# Shots (0=Inf)";
PROGMEM const prog_char _tx_03_bulb[]                    = "Bulb (sec)";
PROGMEM const prog_char _tx_03_hdrStops[]                = "HDR Stops";
PROGMEM const prog_char _tx_03_mirrorLockup[]            = "Mirror Lockup";
PROGMEM const prog_char _tx_03_timeColon[]               = "  :  :";
PROGMEM const prog_char _tx_03_intervalometerPhotoMode[] = "Intervalometer Photo Mode";
PROGMEM const prog_char _tx_03_pressActivateToExit[]     = "Press Activate to Exit";
PROGMEM const prog_char _tx_03_noYesList[]               = "No |Yes";
#endif

#ifdef USE_GRAVITYMENU
PROGMEM const prog_char _tx_04_gravityMenu[]             = "Gravity Menu";
PROGMEM const prog_char _tx_04_distance[]                = "Distance";
PROGMEM const prog_char _tx_04_distanceUnits[]           = "Distance Units";
PROGMEM const prog_char _tx_04_shutterLag[]              = "Shutter Lag (ms)";
PROGMEM const prog_char _tx_04_gravityPhotoMode[]        = "Gravity Photo Mode";
PROGMEM const prog_char _tx_04_sensor2DidNotTrigger[]    = " Sensor2 did not trigger ";
PROGMEM const prog_char _tx_04_readyForObject[]          = "  Ready for object!  ";
PROGMEM const prog_char _tx_04_startingHeight[]          = "Starting Height: ";
PROGMEM const prog_char _tx_04_inch[]                    = " inch";
PROGMEM const prog_char _tx_04_cm[]                      = " cm";
PROGMEM const prog_char _tx_04_sensorTime[]              = "Sensor Time: ";
PROGMEM const prog_char _tx_04_ms[]                      = " ms";
PROGMEM const prog_char _tx_04_fallTime[]                = "Fall Time: ";
PROGMEM const prog_char _tx_04_pressActivateToExit[]     = "Press Activate to Exit";
PROGMEM const prog_char _tx_04_inchCmList[]              = "Inch|Cm  ";
#endif

#ifdef USE_STACKERMENU
PROGMEM const prog_char _tx_05_stackerMenu[]             = "Stacker Menu";
PROGMEM const prog_char _tx_05_motorLoc[]                = "Motor Location";
PROGMEM const prog_char _tx_05_picsStack[]               = "Photos/Stack";
PROGMEM const prog_char _tx_05_motorDir[]                = "Motor Direction";
PROGMEM const prog_char _tx_05_distUnit[]                = "Distance Unit";
PROGMEM const prog_char _tx_05_stepsUnit[]               = "Steps/Unit";
PROGMEM const prog_char _tx_05_totalDist[]               = "Total Distance";
PROGMEM const prog_char _tx_05_settle[]                  = "Settle Time(ms)";
PROGMEM const prog_char _tx_05_speedStepsSec[]           = "Speed(Steps/Sec)";
PROGMEM const prog_char _tx_05_stackerPhotoMode[]        = "Stacker Photo Mode";
PROGMEM const prog_char _tx_05_photosTaken[]             = "Photos Taken";
PROGMEM const prog_char _tx_05_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const prog_char _tx_05_photoStackComplete[]      = "  Photo Stack Complete!!";
PROGMEM const prog_char _tx_05_locationList[]            = "SENSOR 1|SENSOR 2";
PROGMEM const prog_char _tx_05_unitsList[]               = "INCHES|CM    ";
PROGMEM const prog_char _tx_05_dirList[]                 = "FORWARD|REVERSE";
#endif

#ifdef USE_PANORAMAMENU
PROGMEM const prog_char _tx_06_panoramaMenu[]            = "Panorama Menu";
PROGMEM const prog_char _tx_06_motorLoc[]                = "Motor Location";
PROGMEM const prog_char _tx_06_picsPano[]                = "Photos/Pano";
PROGMEM const prog_char _tx_06_motorDir[]                = "Motor Direction";
PROGMEM const prog_char _tx_06_stepsRevolution[]         = "Steps/Revolution";
PROGMEM const prog_char _tx_06_totDegrees[]              = "Total Degrees";
PROGMEM const prog_char _tx_06_settle[]                  = "Settle Time(ms)";
PROGMEM const prog_char _tx_06_speedStepsSec[]           = "Speed(Steps/Sec)";
PROGMEM const prog_char _tx_06_panoramaPhotoMode[]       = "Panorama Photo Mode";
PROGMEM const prog_char _tx_06_photosTaken[]             = "Photos Taken";
PROGMEM const prog_char _tx_06_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const prog_char _tx_06_panoramaComplete[]        = "   Panorama Complete!!!";
PROGMEM const prog_char _tx_06_locationList[]            = {"SENSOR 1|SENSOR 2"};
PROGMEM const prog_char _tx_06_dirList[]                 = {"FORWARD|REVERSE"};
#endif

#ifdef USE_JOGMENU
PROGMEM const prog_char _tx_07_jogMotorMenu[]            = "Jog Motor Menu";
PROGMEM const prog_char _tx_07_motorLocation[]           = "Motor Location";
PROGMEM const prog_char _tx_07_stepsJogLR[]              = "Steps/Jog(L/R)";
PROGMEM const prog_char _tx_07_stepsJogUD[]              = "Steps/Jog(U/D)";
PROGMEM const prog_char _tx_07_speed[]                   = "Speed(Steps/Sec)";
PROGMEM const prog_char _tx_07_jogMotorMode[]            = "Jog Motor Mode";
PROGMEM const prog_char _tx_07_direction[]               = "Direction";
PROGMEM const prog_char _tx_07_jogMode[]                 = "Jog Mode";
PROGMEM const prog_char _tx_07_stepsJog[]                = "Steps/Jog";
PROGMEM const prog_char _tx_07_totSteps[]                = "Tot Steps";
PROGMEM const prog_char _tx_07_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const prog_char _tx_07_forward[]                 = "Forward ";
PROGMEM const prog_char _tx_07_backward[]                = "Backward";
PROGMEM const prog_char _tx_07_single[]                  = "Single    ";
PROGMEM const prog_char _tx_07_continuous[]              = "Continuous";
PROGMEM const prog_char _tx_07_locationList[]            = "SENSOR 1|SENSOR 2";
#endif
/////////// Start developer strings ///////////////////////////////////////////

///////////// End developer strings ///////////////////////////////////////////

char *getStr(const char* inStr)
{
  byte i = -1;

  // Search for null terminated string or use the special '|' char to terminate a string
  // The '|' termination makes it easier to do lists of strings in PROGMEM
  do {
    ++i;
    g_cbuf[i] = pgm_read_byte_near(inStr+i);
  }while (g_cbuf[i] != '\0' && g_cbuf[i] != '|');
 
  g_cbuf[i] = '\0';
  return g_cbuf;
}
