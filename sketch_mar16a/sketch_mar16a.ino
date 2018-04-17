/*
 Using a single switch to select between 3 modes
*/
// Schematic: http://www.pwillard.com/files/mode4.jpg
//===============================================================
// Global Variables & Constants
//===============================================================
 
const int ledPinOne = 13; // LED1 ANODE
const int ledPinTwo = 11; // LED2 ANODE
//const int ledPinThree = 11; // LED3 ANODE
const int modePin = 7; // Active HIGH, held low by 4.7K
 
int mode = 0; // Selector State (Initial state = ALL OFF)
int val = 0; // Pin 13 HIGH/LOW Status
int butState = 0; // Last Button State
int modeState = 0; // Last Mode State
boolean debug = 1; // 1 = Print Serial Enabled / 0 = disabled

//FRONT

// include the necessary libraries
#include <SPI.h>
#include <SD.h>
#include <TFT.h>  // Arduino LCD library

// pin definition for the Uno
#define sd_cs  4
#define lcd_cs 10
#define dc     9
#define rst    8

TFT TFTscreen = TFT(lcd_cs, dc, rst);
PImage logo;

char sensorPrintout[4];
  
//===============================================================
// SETUP
//===============================================================
void setup () {
 pinMode(ledPinOne, OUTPUT);
 pinMode(ledPinTwo, OUTPUT);
 //pinMode(ledPinThree, OUTPUT);
 pinMode(modePin, INPUT);
 if (debug){
 Serial.begin(9600);
 Serial.print("Initial Mode: ");
 Serial.println(mode);
 Serial.print("Setup Complete\n");
 // initialize the serial port: it will be used to
  // print some diagnostic info
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  // clear the GLCD screen before starting
  TFTscreen.background(255, 255, 255);

  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(sd_cs)) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("OK!"));

  // initialize and clear the GLCD screen
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
 }
}
 
//===============================================================
// Main Loop
//===============================================================
void loop() {
 
 val = digitalRead(modePin);
 
 // If we see a change in button state, increment mode value
 if (val != butState && val == HIGH){
 mode++;
 }
 
 butState = val; // Keep track of most recent button state
 
 // No need to keep setting pins *every* loop
 if (modeState != mode){
 
 // If no keys have been pressed yet don't execute
 // the switch code below
 // if (mode != 0) {
 
 switch ( mode ) {
 //case 1 is actually handled below as default
 
case 2:
 TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
 

  logo = TFTscreen.loadImage("FRONTS~1.BMP");
  if (!logo.isValid()) {
    Serial.println(F("error while loading arduino.bmp"));
  }
 TFTscreen.image(logo, 20, 0);

  TFTscreen.setTextSize(4);




  // Read the value of the sensor on A0
  String sensorVal1 = String(analogRead(A0));

  // convert the reading to a char array
  sensorVal1.toCharArray(sensorPrintout, 4);

  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value

  TFTscreen.text(sensorPrintout, 47, 45);
 
  // wait for a moment
   delay(1000);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 47, 45);
 showState();
 break;
 case 3:
  logo = TFTscreen.loadImage("REARSV~1.BMP");
  if (!logo.isValid()) {
    Serial.println(F("error while loading arduino.bmp"));
  }
 TFTscreen.image(logo, 20, 0);

  TFTscreen.setTextSize(4);

  // Read the value of the sensor on A1
  String sensorVal2 = String(analogRead(A1));

  // convert the reading to a char array
  sensorVal2.toCharArray(sensorPrintout, 4);

  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value

  TFTscreen.text(sensorPrintout, 47, 45);
 
  // wait for a moment
  delay(1000);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 47, 45);
 showState();
 break;
 case 4:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(4);
  // write the text to the top left corner of the screen
  TFTscreen.text("PLEASE ", 10, 0);
  TFTscreen.setTextSize(3);
  TFTscreen.text("TO PARK! ", 10, 100);



  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value
  TFTscreen.setTextSize(5);
  TFTscreen.text("N ", 60, 40);
   
      
  // wait for a moment
  delay(250);
  // erase the text you just wrote
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 50, 50);
    TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 30, 70);
    TFTscreen.stroke(0, 0, 0);
 showState();
 break;
 default:
 mode = 1;
 // loop back to 1 by default, seems redundant but
 // it also handles the "mode is > 3" problem
 TFTscreen.begin();
  TFTscreen.background(0, 0, 0);

  // now that the SD card can be access, try to load the
  // image file.
  logo = TFTscreen.loadImage("DRIVES~1.BMP");
  if (!logo.isValid()) {
    Serial.println(F("error while loading arduino.bmp"));
  }
 TFTscreen.image(logo, 20, 0);

  
  // Read the value of the sensor on A2
  String sensorVal3 = String(analogRead(A2));

  // convert the reading to a char array
  sensorVal3.toCharArray(sensorPrintout, 4);


  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value
TFTscreen.setTextSize(3);
  TFTscreen.text(sensorPrintout, 56, 20);
  TFTscreen.setTextSize(4);
  TFTscreen.text(sensorPrintout, 49, 45);
  TFTscreen.setTextSize(3);
  TFTscreen.text(sensorPrintout, 57, 80);
 
  // wait for a moment
   delay(1000);
   TFTscreen.setTextSize(3);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 56, 20);
  TFTscreen.setTextSize(4);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 49, 45);
  TFTscreen.setTextSize(3);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 57, 80);
  
 showState();
 break;
 } // end switch
// } // end of "if mode = 0" check
 } // end of ModeState check
 modeState = mode; // Keep track of mode recent mode value
 delay(10); // slow the loop just a bit for debounce
}
 
//===============================================================
// Subroutine
//===============================================================
void showState() {
 if (debug){
 Serial.print("Mode: ");
 Serial.println(mode);
 }
}
