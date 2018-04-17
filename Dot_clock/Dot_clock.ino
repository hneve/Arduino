/*
Dot Clock

Rtc clock with 4x8x8 dot matrix display and DHT22 temp/humid sensor.
Both SPI 
RTC ss 8
8x8 ss 10

*/

/* TOP SECTION
 * Includes, Defines & inits
 * */
#define F_CPU 8e6

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22

#define Hour	2
#define Minute	1
#define Seconds 0
#define Day		4
#define Month	5
#define Year	6
#define DOT_cs  10 // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
#define RTC_cs  8
#define numberOfHorizontalDisplays 4
#define numberOfVerticalDisplays   1

String tape;
int wait = 20; // In milliseconds
int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

DHT dht(DHTPIN, DHTTYPE);
Max72xxPanel matrix = Max72xxPanel(DOT_cs, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void setup() {
	matrix.setIntensity(2); // Use a value between 0 and 15 for brightness
	matrix.setRotation(0, 3);    // The first display is position upside down
	matrix.setRotation(1, 3);    // The same hold for the last display
	matrix.setRotation(2, 3);
	matrix.setRotation(3, 3);
	RTC_init();
	dht.begin();
	Serial.begin(9600);
}

void loop() {
// Check serial	
	if(Serial.available()) {
		tape = Serial.readString();
		Serial.println(tape);
	}
	else {
		tape = readRTC();	
    //tape="Hello";
	}
// Print text tape on matrix display
	for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {
		matrix.fillScreen(LOW);
		int letter = i / width;
		int x = (matrix.width() - 1) - i % width;
		int y = 0; //(matrix.height() - 8) / 2; // center the text vertically
		while ( x + width - spacer >= 0 && letter >= 0 ) {
			if ( letter < tape.length() ) {
				matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
			}
			letter--;
			x -= width;
		}
		matrix.write(); // Send bitmap to display
		delay(wait);
	}
}

int RTC_init(){ 
//Setup RTC communication.
	pinMode(RTC_cs,OUTPUT); // chip select
// start the SPI library:
	SPI.begin();
	SPI.setBitOrder(MSBFIRST); 
	SPI.setDataMode(SPI_MODE3); // both mode 1 & 3 should work 
//set control register 
	digitalWrite(RTC_cs, LOW);  
	SPI.transfer(0x8E);
	SPI.transfer(0x60); //60= disable Oscillator and Battery SQ wave @1hz, temp compensation, Alarms disabled
	digitalWrite(RTC_cs, HIGH);
	//UBRR0 = 0; 
	delay(10);
}

String readRTC(){
	int TimeDate [7]; //second,minute,hour,Dayofweek,day,month,year
	String temp;	//Placeholder 
//Get all time data from RTC
	digitalWrite(RTC_cs, LOW);
	SPI.transfer(0x00); 
	for(int x=0 ; x <= 6 ; x++){ 
		TimeDate[x] = SPI.transfer(0x00);
		TimeDate[x] = (TimeDate[x] & 0x0f) + ((TimeDate[x] >> 4) * 10);     
	}
	digitalWrite(RTC_cs, HIGH);
//Get DHT data 
	int cel = dht.readTemperature(0);
	int rel = dht.readHumidity();
	if (TimeDate[Hour] < 10) temp.concat("0");
	temp.concat(TimeDate[Hour]);
	temp.concat(":") ;
	if (TimeDate[Minute] < 10) temp.concat("0");
	temp.concat(TimeDate[Minute]);
	temp.concat(":") ;
	if (TimeDate[Seconds] < 10) temp.concat("0");
	temp.concat(TimeDate[Seconds]);
	temp.concat(" ") ;
	if (TimeDate[Day] < 10) temp.concat("0");
	temp.concat(TimeDate[Day]);
	temp.concat(".") ;
	if (TimeDate[Month] < 10) temp.concat("0");
	temp.concat(TimeDate[Month]);
	temp.concat(".") ;
	temp.concat(TimeDate[Year]);
	temp.concat(" ") ;
	temp.concat(cel);
	temp.concat("'C ") ;
	temp.concat(rel);
	temp.concat("%rh ") ;
	return(temp);
}
