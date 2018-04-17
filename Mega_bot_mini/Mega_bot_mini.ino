#include <Servo.h> 
Servo right, left;  // create servo object to control a servo 
#include "Wire.h"
#include "WiiChuck.h"
#define MAXANGLE 90
#define MINANGLE -90
WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
int right_eng_zero = 80;
int left__eng_zero = 79;
int  run_right;
int run_left;
int steer = 0;
int run = 0;
int maximum= 50;

void setup() 
{ Serial.begin(9600);  
  chuck.begin();
  chuck.update();
  chuck.calibrateJoy();
  right.attach(9);  // attaches the servo on pin 9 to the servo object 
  left.attach(10);
  right.write(80);
  left.write(80);
} 
 
void loop() 
{ 
  
  chuck.update();
  delay(10);
  steer = map(chuck.readJoyX(),-127,128,(-maximum+1)/2,maximum/2);
  run   = map(chuck.readJoyY(),-127,128,-maximum+1,maximum);
  run_right = run;
  run_left = run;
  
   if(steer < 0) {
       run_left = run + steer;
   }
   else {
     run_right = run - steer;
   }
    left.write(run_left  + left__eng_zero);
    right.write(run_right + right_eng_zero);
  
  
} 
