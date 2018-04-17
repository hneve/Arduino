////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010, 2011, 2012)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This sensor is times a solenoid valve opening to create one or two drops and take a picture.
// Device 1 is optionally attached to your camera (alternately you can trigger the camera manually with a long exposure)
// Here is the normal sequence 
//   Drop 1 is created
//   Wait and then create Drop 2
//   Wait and then trigger device 2 (flash)

#ifdef USE_VALVEMENU
////////////////////////////////////////
// Function: valveMenu - Handles menuing for the valve sensor
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void valveMenu()
{
  byte button     = BUTTON_NONE;
  byte maxChars   = 3;  // Max number of characters in edit fields
  byte yAdj = 0;
  
  if (g_mPos < 6)
    yAdj = 0;
  else if (g_mPos < 10)
    yAdj = 63;
  else
    yAdj = 99;
  
  printMenuTitle(_tx_02_valveMenu, maxChars, yAdj);
  printMenuName(_tx_02_v1Drop1Size,        0, yAdj);
  printMenuName(_tx_02_v1Drop2Delay,       1, yAdj);
  printMenuName(_tx_02_v1Drop2Size,        2, yAdj);
  printMenuName(_tx_02_v1Drop3Delay,       3, yAdj);
  printMenuName(_tx_02_v1Drop3Size,        4, yAdj);
  printMenuName(_tx_02_flashDelay,         5, yAdj);

  printMenuName(_tx_02_v2StartOffset,      6, yAdj);
  printMenuName(_tx_02_v2Drop1Size,        7, yAdj);
  printMenuName(_tx_02_v2Drop2Delay,       8, yAdj);
  printMenuName(_tx_02_v2Drop2Size,        9, yAdj);

  printMenuName(_tx_02_flashDelayAutoInc, 10, yAdj);
  printMenuName(_tx_02_numShots,          11, yAdj);
  printMenuName(_tx_02_secBetwnShots,     12, yAdj);
  printMenuName(_tx_02_repeatShots,       13, yAdj);

  
  // Draw the changable values in this menu (only one can be selected at a time)
  button |= printMenuSetNumber(maxChars, 0, g_mPos == 0, EEPROM_VALVE_V1_DROP1_SIZE,      3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 1, g_mPos == 1, EEPROM_VALVE_V1_DROP2_DELAY,     3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 2, g_mPos == 2, EEPROM_VALVE_V1_DROP2_SIZE,      3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 3, g_mPos == 3, EEPROM_VALVE_V1_DROP3_DELAY,     3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 4, g_mPos == 4, EEPROM_VALVE_V1_DROP3_SIZE,      3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 5, g_mPos == 5, EEPROM_VALVE_FLASH_DELAY,        3, 0 , 9, yAdj);

  button |= printMenuSetNumber(maxChars, 6, g_mPos == 6, EEPROM_VALVE_V2_START_OFFSET,    3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 7, g_mPos == 7, EEPROM_VALVE_V2_DROP1_SIZE,      3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 8, g_mPos == 8, EEPROM_VALVE_V2_DROP2_DELAY,     3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars, 9, g_mPos == 9, EEPROM_VALVE_V2_DROP2_SIZE,      3, 0 , 9, yAdj);
  
  button |= printMenuSetNumber(maxChars,10, g_mPos ==10, EEPROM_VALVE_FLASH_INCREMENT,    2, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars,11, g_mPos ==11, EEPROM_VALVE_NUM_SHOTS,          3, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars,12, g_mPos ==12, EEPROM_VALVE_SEC_BETWEEN_SHOTS,  2, 0 , 9, yAdj);
  button |= printMenuSetNumber(maxChars,13, g_mPos ==13, EEPROM_VALVE_REPEAT_SHOTS,       1, 0 , 9, yAdj);

  menuProcessButton(button);
  menuScroll(button, 14);
}

////////////////////////////////////////
// Function: valveFunc - Handles triggering for valve menu
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void valveFunc()
{
  unsigned int numShots      = eepromReadInt(EEPROM_VALVE_NUM_SHOTS);
  unsigned int v1Drop1Size   = eepromReadInt(EEPROM_VALVE_V1_DROP1_SIZE);
  unsigned int v1Drop2Delay  = eepromReadInt(EEPROM_VALVE_V1_DROP2_DELAY);
  unsigned int v1Drop2Size   = eepromReadInt(EEPROM_VALVE_V1_DROP2_SIZE);
  unsigned int v1Drop3Delay  = eepromReadInt(EEPROM_VALVE_V1_DROP3_DELAY);
  unsigned int v1Drop3Size   = eepromReadInt(EEPROM_VALVE_V1_DROP3_SIZE);
  unsigned int v2StartOffset = eepromReadInt(EEPROM_VALVE_V2_START_OFFSET);
  unsigned int v2Drop1Size   = eepromReadInt(EEPROM_VALVE_V2_DROP1_SIZE);
  unsigned int v2Drop2Delay  = eepromReadInt(EEPROM_VALVE_V2_DROP2_DELAY);
  unsigned int v2Drop2Size   = eepromReadInt(EEPROM_VALVE_V2_DROP2_SIZE);
  unsigned int flashDelay    = eepromReadInt(EEPROM_VALVE_FLASH_DELAY);
  unsigned int flashInc      = eepromReadInt(EEPROM_VALVE_FLASH_INCREMENT);
  unsigned int secBetwnShots = eepromReadInt(EEPROM_VALVE_SEC_BETWEEN_SHOTS);
  unsigned int repeatShots   = eepromReadInt(EEPROM_VALVE_REPEAT_SHOTS) + 1;
  unsigned int t11on, t11off, t12on, t12off, t21on, t21off, t22on, t22off, maxtime, elapsedMilli;
  unsigned int t13on, t13off;
  unsigned long start;
  unsigned int repeat;

  attachInterrupt(0, activeButtonISR, LOW);
  numShots = (numShots == 0) ? 1 : numShots;

  do
  {
    --numShots;
    
    for(repeat = 0; repeat < repeatShots; ++repeat)
    {
  
      g_dogm.start();
      do 
      {
        printMenuTitle(_tx_02_valvePhotoMode, 0xFF, 0);
        printMenuName(_tx_02_valveActive, 5, 0);
      } 
      while( g_dogm.next() );
    
      setSensorPinModes(0, OUTPUT, INPUT);
      setSensorPinModes(1, OUTPUT, INPUT);
      setSensorPins(0, LOW, LOW);
      setSensorPins(1, LOW, LOW);
    
      setDevicePins(0, HIGH, HIGH);  // Trigger camera
      delay(400);// Give camera 400 ms to open shutter
    
      t11on  = 0;
      t11off = t11on  + v1Drop1Size;
      t12on  = t11off + v1Drop2Delay;
      t12off = t12on  + v1Drop2Size;
      t13on  = t12off + v1Drop3Delay;
      t13off = t13on  + v1Drop3Size;
    
      t21on  = t11on  + v2StartOffset;
      t21off = t21on  + v2Drop1Size;
      t22on  = t21off + v2Drop2Delay;
      t22off = t22on  + v2Drop2Size;
    
      maxtime      = max(t13off, t22off);
    
      elapsedMilli = 0;
      start        = millis();
      
      while (start == millis())  // This makes sure we are starting at the beginning of a millisecond for consistency purposes
      {}
      start        = millis();
     
      setSensorPins(0, HIGH, LOW);  // Turn on V1
     
      //Loop for triggering all valves events
      while (elapsedMilli <= maxtime)
      {
        elapsedMilli = millis() - start;
        if (elapsedMilli >= t13off)
          setSensorPins(0, LOW, LOW);   // Turn off V1, drop 3
        else if (elapsedMilli >= t13on)
          setSensorPins(0, HIGH, LOW);  // Turn on V1, drop 3
        else if (elapsedMilli >= t12off)
          setSensorPins(0, LOW, LOW);   // Turn off V1
        else if (elapsedMilli >= t12on)
          setSensorPins(0, HIGH, LOW);  // Turn on V1
        else if (elapsedMilli >= t11off)
          setSensorPins(0, LOW, LOW);   // Turn off V1
    
    
        if (elapsedMilli >= t22off)
          setSensorPins(1, LOW, LOW);   // Turn off V2
        else if (elapsedMilli >= t22on)
          setSensorPins(1, HIGH, LOW);  // Turn on V2
        else if (elapsedMilli >= t21off)
          setSensorPins(1, LOW, LOW);   // Turn off V2
        else if (elapsedMilli >= t21on)
          setSensorPins(1, HIGH, LOW);  // Turn on V2
      }
     
      //Wait for Flash Delay and then Trigger flash
      delay(flashDelay);
    
      // Trigger flash
      setDevicePins(1, HIGH, HIGH);
      delay(FLASH_DURATION_MS);
      setDevicePins(1, LOW, LOW);  // Turn Flash off
      setDevicePins(0, LOW, LOW);  // Turn Camera off
  
      for (unsigned int i = 0; i < secBetwnShots; ++i)
      {
        if ( g_menuMode == MENUMODE_MENU )
          break;
          
        delay(1000);
      }
    }

    if (eepromReadInt(EEPROM_VALVE_FLASH_INCREMENT))
    {
      eepromWriteInt(EEPROM_VALVE_FLASH_DELAY, flashDelay+flashInc);
      eepromClamp(EEPROM_VALVE_FLASH_DELAY, 0, 999);
      flashDelay = eepromReadInt(EEPROM_VALVE_FLASH_DELAY);
    }

  } while ( (numShots > 0) && (g_menuMode != MENUMODE_MENU) );
  
  g_menuMode = MENUMODE_MENU;  // Always return to menu in this mode
  detachInterrupt(0);
}
#endif

