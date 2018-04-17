// Project Fotobox 
// Auth: Hans . Neve (at) gmail.com
// www.neve.nu
// 
// A new start of fotoino project started 4.1.13
// 
// High speed flash trigger
// Camera trigger - function
// Timelapse - function
// External source trigger and timing for flash and/or camera

/*
HARDWARE SETUP:
Buttons connected to PORTD pins 0,1,2,3.
Optocoupler to PORTB pins 2,3,4,5.

Sensor pin is PORTC pin 0 aka A0.

LCD connected to pins 9,8,7,6,5,4 aka PORT: b1,b0,d7,d6,d5,d4


*/

// TOP SECTION .
//DEFINE bit manipulation:
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define SETBITMASK(x,y) (x |= (y))
#define CLEARBITMASK(x,y) (x &= (~y))
#define FLIPBITMASK(x,y) (x ^= (y))
#define CHECKBITMASK(x,y) (x & (y))

//Define outputs
#define SHUTTER_FLIP	FLIPBIT(PORTB,5) //13
#define FOCUS_FLIP		FLIPBIT(PORTB,4) //12
#define FLASH_FLIP		FLIPBIT(PORTB,3) //11
#define EXTERNAL_FLIP	FLIPBIT(PORTB,2) //10

//Define inputs 
#define D_SENSOR	CHECKBIT(PINC,0) // A0-aka14
#define BTN1		!CHECKBIT(PIND,0)	 // All buttons are active low - thus inverted.
#define BTN2		!CHECKBIT(PIND,1)
#define BTN3		!CHECKBIT(PIND,2)
#define BTN4		!CHECKBIT(PIND,3)
#define PIN_A_SENSOR	A0

//Includes and variables:
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(9,8,7,6,5,4);	// Setup LiquidCrystal aka PORT: b1,b0,d7,d6,d5,d4

char		lcdLine1[17];
char		lcdLine2[17];
uint16_t	analogLevel;
uint16_t	analogLevelHigh;
uint16_t	analogLevelLow;
uint8_t		analogLevelThreshold = 10;
bool		digitalLevel;
bool		btnPlus;
bool		btnMinus;
bool		btnRight;
bool		btnLeft;
bool		sensorAnalog = true;
uint16_t	delayTime[2];
uint8_t		delayIncrement = 1;
uint8_t		drops = 1;
uint8_t		dropSize = 10;
uint8_t		mode = 0; 
uint8_t		menu = 0;
uint8_t		menuEnd = 6;
uint8_t		menuItem=0;
uint8_t		menuLine[4][7] = {{0,1,2,6,0,0,0},{0,1,2,6,0,0,0},{0,2,6,0,0,0,0},{0,1,2,3,4,5,6}};
long		btnOldTime;
uint16_t	buttonRepeatDelay;
uint16_t	btnHeld;
uint8_t		button;
uint8_t		buttonOld;

// #include "lcd_drv\main.h"
// #include "lcd_drv\lcd_drv.h"
// #include "lcd_drv\lcd_drv.
void setup(){
	ADCSRA |=  (1 << ADPS2);		// ADC prescaler 16
	SETBITMASK(DDRB,0b00111100);	//set portB 5,4,3,2 to outputs for optocoupler
	SETBITMASK(PORTD, 0b00001111);	//Internal pull ups for buttons
	//lcd.begin(16,2);
	lcd_init();
	lcd_puts( "Line 1" );
	//lcd.print("FotoBox v.0.6");
	delay(1500);
	//lcd.clear();
        if(BTN4) systemTest();
		digitalWrite(1,1);
}

void loop(){
	switch(menu){
		case 0:{setMode();break;}
		case 1:{sensorSetup();break;}
		case 2:{setDelay(0);break;}
		case 3:{setDelay(1);break;}
		case 4:{setDrops();break;}
		case 5:{setDropsize();break;}
		case 6:{run();break;}
	}
	updateLcd();
	getButtons();
}

void getButtons(){
	button = ((BTN4 << 3) | (BTN3 << 2) | (BTN2 << 1) | (BTN1 << 0));
	if (button == buttonOld){
		btnHeld++;
		if(millis() <= (btnOldTime + buttonRepeatDelay)) {return;}	
	}
	//else {btn_held = 0;}
	
	switch (button){
		case 1:
			btnRight = true;
			buttonRepeatDelay = 500;
			break;
		case 2:
			btnLeft = true;
			buttonRepeatDelay = 500;
 			break;
		case 4:
			btnPlus=true;
			if(btnHeld > 400) buttonRepeatDelay = 50;
			break;
		case 8:
			btnMinus=true;
			if(btnHeld > 400) buttonRepeatDelay = 50;
			break;
		default:
			btnHeld =0;
			buttonRepeatDelay = 500;
			break;
	}	
	buttonOld = button;
	btnOldTime = millis();
	
}

void updateLcd(){
	lcd.setCursor(0,0);
	if(menu) lcd.print("<"); 
		else lcd.print("#");
	lcd.setCursor(15,0);
	if(menu == menuEnd) lcd.print("#"); 
		else lcd.print(">");
	lcd.setCursor(2,0);
	lcd.print(lcdLine1);
	
	lcd.setCursor(0,1);
	lcd.print(lcdLine2);
}

void setMode(){
	if(btnPlus){
		mode++;
		btnPlus=false;
		} 
	if(btnMinus){
		mode--;
		btnMinus=false;
	} 
	if(btnRight){
		menu = menuLine[mode][++menuItem];
		btnRight=false;
	} 
	if(btnLeft){ 
		// Does nothing
		btnLeft=false;
	}
	
	mode = constrain(mode,0,3);
	sprintf(lcdLine1,"MODE        ");
	if(mode == 0) sprintf(lcdLine2,"Flashtrigger    ");	
	if(mode == 1) sprintf(lcdLine2,"Cameratrigger   ");
	if(mode == 2) sprintf(lcdLine2,"Timelapse       ");	
	if(mode == 3) sprintf(lcdLine2,"Ext Control     ");	
}
void sensorSetup(){
	if(btnPlus & !sensorAnalog){	
		sensorAnalog = true;
		btnPlus = false;
	}
	if(btnPlus & sensorAnalog){
		switch(analogLevelThreshold){
			case 1:	
				analogLevelThreshold = 10;
				break;
			case 10: 
				analogLevelThreshold = 100;
				break;
			case 100: 
				analogLevelThreshold = 1;
				break;
		}
		btnPlus = false;
	}
	if(btnMinus){	
		sensorAnalog = false;  
		btnMinus = false;
	}
	if(btnRight){ 
		menu = menuLine[mode][++menuItem];
		btnRight=false;
	}
	if(btnLeft){ 
		menu = menuLine[mode][--menuItem];
		btnLeft=false;
	}
	
	sprintf(lcdLine1,"SENSOR SETUP");
	if(sensorAnalog){
		analogLevel = analogRead(PIN_A_SENSOR);
		sprintf(lcdLine2, "A:%4i   T:%3i", analogLevel,analogLevelThreshold);
	}
	else{
		if(D_SENSOR) digitalLevel = 'H'; else digitalLevel = 'L';
		sprintf(lcdLine2, "D:%c             ", digitalLevel);
	}
}

void setDelay(int nr){
	
	if(btnPlus ){ 
		delayTime[nr]+=delayIncrement;
		delayTime[nr]=constrain(delayTime[nr],0,9999);
		btnPlus  = false;
	}
	if(btnMinus){ 
		delayTime[nr]-=delayIncrement;
		delayTime[nr]=constrain(delayTime[nr],0,9999);
		btnMinus = false;
	}
	if(btnRight){ 
		menu = menuLine[mode][++menuItem];
		btnRight =false;
	}
	if(btnLeft ){ 
		menu = menuLine[mode][--menuItem];
		btnLeft = false;	
	}
	
	sprintf(lcdLine1,"Delay%i x%4i",nr,delayIncrement);
	sprintf(lcdLine2,"%4i            ",delayTime[nr]);
}
void setDrops(){
	if(btnPlus) drops++;drops = constrain(drops,1,10);btnPlus = false;
	if(btnMinus) drops--;drops = constrain(drops,1,10);btnMinus = false;
	if(btnRight) menu = menuLine[mode][++menuItem];btnRight=false;
	if(btnLeft) menu = menuLine[mode][--menuItem];btnLeft=false;
	sprintf(lcdLine1,"Drops 1-10");
	sprintf(lcdLine2,"%4i",drops);	
}
void setDropsize(){
	if(btnPlus) dropSize++;dropSize = constrain(dropSize,1,100);btnPlus = false;
	if(btnMinus) dropSize--;dropSize = constrain(dropSize,1,100);btnMinus = false;
	if(btnRight) menu = menuLine[mode][++menuItem];btnRight=false;
	if(btnLeft) menu = menuLine[mode][--menuItem];btnLeft=false;
	sprintf(lcdLine1,"Drop size");
	sprintf(lcdLine2,"%4i",dropSize);
}
void run(){
	sprintf(lcdLine1,"RUN?        ");
	sprintf(lcdLine2,"            ");
	
	if(btnPlus){
		btnPlus = false;
		
		switch (mode){
			case 0:	if(sensorAnalog){runAnalogFlashtrigger();}
					else{runDigitalFlashtrigger();}
					break;
			case 1: if(sensorAnalog){runAnalogCameratrigger();}
					else{runDigitalCameratrigger();}
					break;
			case 2: runTimelapse();
					break;
			
		}
	}
	if(btnLeft){ 
		menu = menuLine[mode][--menuItem];
		btnLeft=false;
	}
	if(btnRight) btnRight=false;
}
void calibrate(int time){
	// Calibrate analog input to create high and low trigger level
	lcd.clear();
	time *= 1000;
	lcd.print("Calibrating");

	int level = analogRead(PIN_A_SENSOR);
	analogLevelLow = level-1;
	analogLevelHigh = level+1;
	
	unsigned long timestamp = millis() + time;
	
	while (millis() < timestamp) {
		level = analogRead(PIN_A_SENSOR);
		if (level < analogLevelLow)	{ analogLevelLow  = level;}
		if (level > analogLevelHigh)	{ analogLevelHigh = level;}
		lcd.setCursor(13,0);
		lcd.print((timestamp - millis()) / 1000);
		lcd.setCursor(0,1);
		lcd.print("H");
		lcd.print(analogLevelHigh);
		lcd.print(" L");
		lcd.print(analogLevelLow);
	}
	
	// Add extra threshold level to fluctuation.
	analogLevelLow -= analogLevelThreshold;
	analogLevelHigh += analogLevelThreshold;
	lcd.clear();
}

void runAnalogFlashtrigger(){
	calibrate(5);
	printRunning();
	FOCUS_FLIP;
	SHUTTER_FLIP;
	analogLevel = analogRead(PIN_A_SENSOR);
	
	// Trigger when analog reading is outside of high/low levels or if Btn4 i pressed.
	while (((analogLevel > analogLevelLow) & (analogLevel < analogLevelHigh)) & !BTN4 ) {
		analogLevel = analogRead(PIN_A_SENSOR);
	}			
	delay(delayTime[0]);
	FLASH_FLIP;
	EXTERNAL_FLIP;
	delay(10);
	FLASH_FLIP;
	EXTERNAL_FLIP;
	SHUTTER_FLIP;
	FOCUS_FLIP;
}	
void runDigitalFlashtrigger(){
	printRunning();
	FOCUS_FLIP;		
	SHUTTER_FLIP;	
	bool dsensor_last = D_SENSOR;
	while((D_SENSOR == dsensor_last) & !BTN4) {}
	delay(delayTime[0]);
	FLASH_FLIP; 
	EXTERNAL_FLIP;
	delay(10);
	FLASH_FLIP; 
	EXTERNAL_FLIP;
	FOCUS_FLIP; 
	SHUTTER_FLIP; 		
}
void runAnalogCameratrigger(){
	calibrate(5);
	printRunning();
	
	analogLevel = analogRead(PIN_A_SENSOR);
	
	while (((analogLevel > analogLevelLow) && (analogLevel < analogLevelHigh)) & !BTN4) { 
		analogLevel = analogRead(PIN_A_SENSOR); 
	}
	
	delay(delayTime[0]);
	SHUTTER_FLIP;
	delay(100);
	SHUTTER_FLIP;
}
void runDigitalCameratrigger(){
	printRunning();
	
	bool dsensor_last = D_SENSOR;
	
	while((D_SENSOR == dsensor_last) & !BTN4) {}
	
	delay(delayTime[0]);
	FOCUS_FLIP;
	SHUTTER_FLIP;
	delay(500);
	SHUTTER_FLIP;
	FOCUS_FLIP;
}
void runTimelapse(){
	printRunning();
	
	static long timelapse_last;
	
	while(!BTN4){
		if(millis() > timelapse_last + (delayTime[0] * 1000) ){
			FOCUS_FLIP;
			SHUTTER_FLIP;
			delay(200);
			SHUTTER_FLIP;
			FOCUS_FLIP;
			timelapse_last = millis();
		}
	}
}
void printRunning(){
	lcd.clear();
	lcd.print("  Running!");
}

void systemTest(){
	lcd.print("System testing:");
	lcd.setCursor(0,1);
	lcd.print("1f 2s 3-1 4-2");
        bool hold = 1;
	while(hold){
		getButtons();
		switch (button){
			case 1: SHUTTER_FLIP;break;
			case 2: FOCUS_FLIP;break;
			case 4: FLASH_FLIP;break;
			case 8: EXTERNAL_FLIP;break;
            case 15: hold = 0;break;
		}
		
	}
}
