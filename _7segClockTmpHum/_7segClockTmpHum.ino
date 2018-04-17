#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <SPI.h>
const int dispCS = 10;
const int clkCS = 9;
const byte MAX7219_REG_NOOP        = 0x0;
// codes 1 to 8 are digit positions 1 to 8
const byte MAX7219_REG_DECODEMODE  = 0x9;
const byte MAX7219_REG_INTENSITY   = 0xA;
const byte MAX7219_REG_SCANLIMIT   = 0xB;
const byte MAX7219_REG_SHUTDOWN    = 0xC;
const byte MAX7219_REG_DISPLAYTEST = 0xF;
const byte blank = 0x0f;
const byte second = 0;
const byte minute = 1;
const byte hour = 2;
const byte dayOfWeek = 3;
const byte date = 4;
const byte month = 5;
const byte year = 6;
int rtcRegister [7]; //second,minute,hour,Dayofweek,day,month,year

void transmitDisplay (const byte reg, const byte data){
	digitalWrite (dispCS, LOW);
	SPI.transfer (reg);
	SPI.transfer (data);
	digitalWrite (dispCS, HIGH);
}  // end of transmitDisplay

void transmitRtc(const byte reg, const byte data){
	digitalWrite(clkCS, LOW);
	SPI.transfer (reg);
	SPI.transfer (data);
	digitalWrite(clkCS, HIGH);
} // end of transmitRtc

int RTC_init(){
	//Setup RTC communication.
	pinMode(clkCS,OUTPUT);
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE3); // both mode 1 & 3 should work
	//set control register
	transmitRtc(0x8E,0x60); //60= disable Oscillator and Battery SQ wave @1hz, temp compensation, Alarms disabled
	delay(10);
}

void readRtc(){
	//Get all time data from RTC
	digitalWrite(clkCS, LOW);
	SPI.transfer(0x00);
	for(int x=0 ; x <= 6 ; x++){
		rtcRegister[x] = SPI.transfer(0x00);
		rtcRegister[x] = (rtcRegister[x] & 0x0f) + ((rtcRegister[x] >> 4) * 10);
	}
	digitalWrite(clkCS, HIGH);
}

void setup (){
	RTC_init();
	dht.begin();
	transmitDisplay (MAX7219_REG_SCANLIMIT, 7);      // show 4 digits
	transmitDisplay (MAX7219_REG_DECODEMODE, 0xFF);  // use digits (not bit patterns)
	transmitDisplay (MAX7219_REG_DISPLAYTEST, 0);    // no display test
	transmitDisplay (MAX7219_REG_INTENSITY, 15);      // character intensity: range: 0 to 15
	transmitDisplay (MAX7219_REG_SHUTDOWN, 1);       // not in shutdown mode (ie. start it up)
	// clear display
	for (int x=1;x<=8;x++){transmitDisplay (x, 0x0f);}
}   // end of setup

void loop (){
	readRtc();
	byte temp1 = (uint8_t)dht.readTemperature()/10;
	byte temp2 = (uint8_t)dht.readTemperature()%10;
	byte hum1  = (uint8_t)dht.readHumidity()/10;
	byte hum2  = (uint8_t)dht.readHumidity()%10;
	
	transmitDisplay (9-1, rtcRegister[hour]/10);
	transmitDisplay (9-2, rtcRegister[hour]%10 );
	transmitDisplay (9-3, rtcRegister[minute]/10);
	transmitDisplay (9-4, rtcRegister[minute]%10);
	if (rtcRegister[second]%10 < 5){//rtcRegister[second] < 30
		transmitDisplay (9-5, rtcRegister[date]/10);
		transmitDisplay (9-6, rtcRegister[date]%10);
		transmitDisplay (9-7, rtcRegister[month]/10);
		transmitDisplay (9-8, rtcRegister[month]%10);
		delay (500);
	}
	else{
		//transmitDisplay (9-5, rtcRegister[second]/10);
		//transmitDisplay (9-6, rtcRegister[second]%10);
		//transmitDisplay (9-7, blank);
		//transmitDisplay (9-8, blank);
		//delay (500);
		
		transmitDisplay (9-5, temp1);
		transmitDisplay (9-6, temp2);
		transmitDisplay (9-7, hum1);
		transmitDisplay (9-8, hum2);
		delay (500);
	}
	
}  // end of loop
