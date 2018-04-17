//
//    FILE: pcf8574_test.ino
//  AUTHOR: Rob Tillaart
//    DATE: 27-08-2013
//
// PUPROSE: demo 
//

#include <PCF8574.h>
#include <Wire.h>

// adjust addresses if needed
//PCF8574 PCF_38(0x27);  // add switches to lines  (used as input)
PCF8574 PCF_39(0x27);  // add leds to lines      (used as output)

void setup()
{
 

  

  PCF_39.write(0, 1);
  for (int i=0; i<7; i++)
  {
    PCF_39.shiftLeft();
    delay(1000);
  }

  for (int i=0; i<7; i++)
  {
    PCF_39.shiftRight();
    delay(1000);
  }

  for (int i=0; i<8; i++)
  {
    PCF_39.write(i, 1);
    delay(1000);
    PCF_39.write(i, 0);
    delay(100);
  }

  for (int i=0; i<8; i++)
  {
    PCF_39.toggle(i);
    delay(100);
    PCF_39.toggle(i);
    delay(100);
  }
}

void loop()
{
  // echos the lines
  
}
//
// END OF FILE
//
