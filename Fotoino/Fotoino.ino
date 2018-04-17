/*################################################
 Fotoino by Hans . Neve (at) gmail.com
 www.neve.nu
 Started 4.1.13
 High speed photo flash trigger
 camera trigger
 and time lapse system
################################################*/

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))



#define flash_high			bitSet(PORTB,3)
#define flash_low			bitClear(PORTB,3)
#define cc_shutter_open		SETBIT(PORTB,5)
#define cc_shutter_closed	CLEARBIT(PORTB,5)
#define cc_focus_on			SETBIT(PORTB,4)
#define cc_focus_off		CLEARBIT(PORTB,4)



#define analog_sensor		A1
#define lcd_backlight_pwm	10  // 0 = off - 255 = full on
#define flash_trigger		11
#define cc_focus			12	
#define cc_trigger			13	
#define mode_sensorinfo		1	
#define mode_flashtrigger	2	
#define mode_cameratrigger	3
#define mode_timelapse		4
#define std_backlight_value 128

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);	// Setup LiquidCrystal
#include <key_read.h>
key_read kb(0,769);						// Setup key_read: Analog pin 0, lcd blinks at 1/1.3 sec = .769 seconds

	
 
int				modeShow           = 0;
int				modeSet            = 0;
boolean			digit_plus         = false;
boolean			digit_minus        = false;
int				menu_mode          = 0;
int				menu_mode_digit    = 0;
int				toleranse          = 10;
unsigned int	laps               = 0; // 0 = infinite
unsigned int	lapsDelay          = 1; //seconds
unsigned int	timeDelay          = 0; //ms
char			buffer [20];
boolean			lcd_redraw         = true;
boolean			run                = false;
unsigned int	level;
int				level_low = 1023;
int				level_high;
unsigned long	timestamp;


void setup(){
	pinMode(flash_trigger, OUTPUT);
	pinMode(cc_focus, OUTPUT);
	pinMode(cc_trigger, OUTPUT);
	analogWrite(lcd_backlight_pwm ,std_backlight_value);
	lcd.begin(16, 2);
	lcd.setCursor(0,0);
	lcd.print("Fotoino V.0.9");
	lcd.setCursor(0,1);
	lcd.print("Hneve 2013");
	delay(2000);
	lcd.clear();
}
void loop(){
	if(modeSet == 0){	
		lcd.setCursor(0,0);
		lcd.print("Set mode:     ");
		lcd.setCursor(0,1);
		if(modeShow == 0) lcd.print("Sensor info   ");
		if(modeShow == 1) lcd.print("Flashtrigger  ");
		if(modeShow == 2) lcd.print("Cameratrigger ");
		if(modeShow == 3) lcd.print("TimeLapse     ");
		
		switch (kb.read()) {
			case btn_up:	 { modeShow++; break;}
			case btn_down:   { modeShow--; break;}
			case btn_right:  { modeSet =  modeShow + 1 ; lcd_redraw = true; break;}
			}
		if(modeShow > 3) modeShow = 0;
		if(modeShow < 0) modeShow = 3;
		}
		if(modeSet == mode_sensorinfo)    mode_sensor_info();
		if(modeSet == mode_flashtrigger)  mode_flash_trigger();
		if(modeSet == mode_cameratrigger) mode_camera_trigger();
		if(modeSet == mode_timelapse)     mode_time_lapse();
	}
void calibrate(int time){
	// Calibrate analog input to create high and low trigger level
	lcd.clear();
	time *= 1000;
	lcd.noBlink();
	lcd.setCursor(0,0);
	lcd.print("Calibrating ");

	int level = analogRead(analog_sensor);
	level_low = level-1;
	level_high = level+1;
	timestamp = millis() + time;
	while (millis() < timestamp) {
		level = analogRead(analog_sensor);
		if (level < level_low) { level_low = level;}
		if (level > level_high) { level_high = level;}
		lcd.setCursor(13,0);
		lcd.print((timestamp - millis()) / 1000);
		lcd.setCursor(0,1);
		lcd.print("H");
		lcd.print(level_high);
		lcd.print(" L");
		lcd.print(level_low);
	}
	level_low -= toleranse;
	level_high += toleranse;
	lcd.clear();
}
void mode_sensor_info(){
	switch (kb.read()){
		case btn_left:	modeSet = 0; break;
		case btn_right: calibrate(5); break;
		case btn_up:	cc_shutter_open;delay(30);cc_shutter_closed;break;			//digitalWrite(cc_trigger,HIGH); delay(30);digitalWrite(cc_trigger,LOW); break;
		case btn_down:	flash_high;delay(30);flash_low;	break;	
	}
	if(millis() >= (timestamp + 250)) {
		level = analogRead(analog_sensor);
		//lcd.noBlink();
		lcd.setCursor(0,0);
		sprintf(buffer, "Sensor:%4i", level);
		lcd.print(buffer);
		lcd.setCursor(0,1);
		lcd.print("                ");
		timestamp = millis();
		}
	}
void mode_flash_trigger(){
	
	if(run) {
		ADCSRA &= ~(0b00000111); //clear ADPS0,1,2 set divider to 2 -  Turbo mode
		calibrate(5);
		analogWrite(lcd_backlight_pwm, 0);     // Turn off backlight LCD
		
		cc_shutter_open;						
		level = analogRead(analog_sensor);
		while ((level > level_low) && (level < level_high)) { level = analogRead(analog_sensor); }
		delay(timeDelay);                      // Trigger delay
			flash_high;
				delay(10);
			flash_low;						
		cc_shutter_closed;							
		
		analogWrite(lcd_backlight_pwm, std_backlight_value);
		ADCSRA |= (0b00000111); //set ADPS0,1,2 - Turbo off
		lcd_redraw = true;
		run = false;
	}

	if(lcd_redraw){
		lcd.noBlink();
		lcd.setCursor(0,0);
		lcd.print("Flashtrigger");
		lcd.setCursor(0,1);
		sprintf(buffer, "D:%4u D:%4i", timeDelay, toleranse);
		lcd.print(buffer);
		lcd_redraw = false;
	}
	

	
	switch (menu_mode_digit) {
		case 0: {
			lcd.noBlink();
			switch (kb.read()) {
				case btn_right:  { menu_mode_digit++; break;}
				case btn_left:	 { modeSet = 0;  break;}
				case btn_select: { run = true;        break;}
			}
			break;}
		
		case 1: { // delay digit 1000
			int inc = 1000;
			lcd.setCursor(2,1);
			lcd.blink();
			switch (kb.read()) {
				case btn_right:  { menu_mode_digit++; break;}
				case btn_left:   { menu_mode_digit--; break;}
				case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
				case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
				case btn_select: { menu_mode_digit = 0; break;}
			}
			break;}
		
		case 2: { // delay digit 100
			int inc = 100;
			lcd.setCursor(3,1);
			lcd.blink();
			switch (kb.read()) {
				case btn_right:  { menu_mode_digit++; break;}
				case btn_left:   { menu_mode_digit--; break;}
				case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
				case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
				case btn_select: { menu_mode_digit = 0; break;}
			}
			break;}
		
		case 3: { // delay digit 10
			int inc = 10;
			lcd.setCursor(4,1);
			lcd.blink();
			switch (kb.read()) {
				case btn_right:  { menu_mode_digit++; break;}
				case btn_left:   { menu_mode_digit--; break;}
				case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
				case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
				case btn_select: { menu_mode_digit = 0; break;}
			}
			break;}
		
		case 4: { // delay digit 1
			int inc = 1;
			lcd.setCursor(5,1);
			lcd.blink();
			switch (kb.read()) {
				case btn_right:  { menu_mode_digit++; break;}
				case btn_left:   { menu_mode_digit--; break;}
				case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
				case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
				case btn_select: { menu_mode_digit = 0; break;}
			}
			break;}
		
		case 5: { // toleranse
			int inc = 10;
			lcd.setCursor(11,1);
			lcd.blink();
			switch (kb.read()) {
				case btn_left:   { menu_mode_digit--; break;}
				case btn_up:     { if(toleranse != 200)toleranse += inc; lcd_redraw = true; break;}
				case btn_down:   { if(toleranse != 0) toleranse -= inc; lcd_redraw = true; break;}
				case btn_select: { menu_mode_digit = 0; break;}
			}
			break;}
		}
		if (timeDelay > 60000) timeDelay = 0;
		if (timeDelay > 9999) timeDelay = 9999;
	}				
void mode_camera_trigger()  {

			if(run) {
				lcd.clear();
				calibrate(5);
				analogWrite(lcd_backlight_pwm, 0);                      // Turn off backlight LCD
				
				level = analogRead(analog_sensor);
				while ((level > level_low) && (level < level_high)) { level = analogRead(analog_sensor); }
				
				delay(timeDelay);
				cc_focus_on; 
					cc_shutter_open;
						delay(600);
					cc_shutter_closed;
				cc_focus_off;
				
				analogWrite(lcd_backlight_pwm, std_backlight_value);
				lcd_redraw = true;
				run = false;
			}

			if(lcd_redraw){
				lcd.noBlink();
				lcd.setCursor(0,0);
				lcd.print("CameraTrigger:");
				lcd.setCursor(0,1);
				lcd.print("D:");
				if (timeDelay > 9999) timeDelay = 9999;
				if (timeDelay < 1000) lcd.print(" ");
				if (timeDelay < 100) lcd.print(" ");
				if (timeDelay < 10) lcd.print(" ");
				lcd.print(timeDelay);
				lcd.print("ms T:");
				lcd.print(toleranse);
				lcd.print(" ");
				lcd_redraw = false;
			}
			
			switch (menu_mode_digit) {
				case 0: {
					lcd.noBlink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:	 { modeSet = 0;		  break;}
						case btn_select: { run = true;        break;}
					}
				break;}
				
				case 1: { // delay digit 1000
					int inc = 1000;
					lcd.setCursor(2,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
				break;}
				
				case 2: { // delay digit 100
					int inc = 100;
					lcd.setCursor(3,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
				break;}
				
				case 3: { // delay digit 10
					int inc = 10;
					lcd.setCursor(4,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
				break;}
				
				case 4: { // delay digit 1
					int inc = 1;
					lcd.setCursor(5,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { timeDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { timeDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
				break;}
				
				case 5: { // toleranse
					int inc = 10;
					lcd.setCursor(11,1);
					lcd.blink();
					switch (kb.read()) {
						//case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(toleranse != 200)toleranse += inc; lcd_redraw = true; break;}
						case btn_down:   { if(toleranse != 0) toleranse -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
				break;}
			}
		}
void mode_time_lapse() {
			unsigned long timedelay = lapsDelay * 1000;

			if(run){
				lcd.setCursor(0,0);
				lcd.print("Timelapse RUN! ");
				lcd.setCursor(0,1);
				sprintf(buffer, "n:%4i D:%4is", laps, lapsDelay);
				lcd.print(buffer);

				if(millis() - timestamp >= timedelay ){
					analogWrite(lcd_backlight_pwm, std_backlight_value);
					
					cc_focus_on;
						cc_shutter_open;
							delay(600);
						cc_shutter_closed;
					cc_focus_off;
					
					analogWrite(lcd_backlight_pwm, 0);
					
					if(laps == 1) { // countdown unless 0  - also end of run. if 0 - never end
						run = false;
						lcd_redraw = true;
					}         
					if(laps) laps--;
					timestamp = millis();
				}
			}

			if(lcd_redraw){
				lcd.clear();
				analogWrite(lcd_backlight_pwm, std_backlight_value);
				lcd.noBlink();
				lcd.setCursor(0,0);
				lcd.print("Timelapse:");
				lcd.setCursor(0,1);
				sprintf(buffer, "n:%4i D:%4is", laps, lapsDelay);
				lcd.print(buffer);
				lcd_redraw = false;
			}

			switch (menu_mode_digit) {
				case 0: {
					lcd.noBlink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:	 { modeSet = 0;  break;}
						case btn_select: { if(run) lcd_redraw = true; run = !run; break;}
					}
					break;}
				
				case 1: { // laps digit 1000
					int inc = 1000;
					lcd.setCursor(2,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(laps != 9 * inc) laps += inc; lcd_redraw = true; break;}
						case btn_down:   { if(laps >= inc) laps -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
					break;}
					
				case 2: { // laps digit 100
					int inc = 100;
					lcd.setCursor(3,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(laps/inc != 9 ) laps += inc; lcd_redraw = true; break;}
						case btn_down:   { if(laps >= inc) laps -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
					break;}
					
				case 3: { // laps digit 10
					int inc = 10;
					lcd.setCursor(4,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(laps/inc != 9) laps += inc; lcd_redraw = true; break;}
						case btn_down:   { if(laps >= inc) laps -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
					break;}
					
				case 4: { // laps digit 1
					int inc = 1;
					lcd.setCursor(5,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(laps/inc != 9) laps += inc; lcd_redraw = true; break;}
						case btn_down:   { if(laps >= inc) laps -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
					break;}
					
				case 5: { // lapsDelay digit 1000
					int inc = 1000;
					lcd.setCursor(9,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(lapsDelay/inc != 9) lapsDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { if(lapsDelay >= inc)   lapsDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
					break;}
					
				case 6: { // lapsDelay digit 100
					int inc = 100;
					lcd.setCursor(10,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(lapsDelay/inc != 9) lapsDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { if(lapsDelay >= inc)   lapsDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
					break;}
					
				case 7: { // lapsDelay digit 10
					int inc = 10;
					lcd.setCursor(11,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  { menu_mode_digit++; break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(lapsDelay/inc != 9) lapsDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { if(lapsDelay >= inc)   lapsDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
					break;}
					
				case 8: { // lapsDelay digit 1
					int inc = 1;
					lcd.setCursor(12,1);
					lcd.blink();
					switch (kb.read()) {
						case btn_right:  {  break;}
						case btn_left:   { menu_mode_digit--; break;}
						case btn_up:     { if(lapsDelay/inc != 9) lapsDelay += inc; lcd_redraw = true; break;}
						case btn_down:   { if(lapsDelay-1 >= inc) lapsDelay -= inc; lcd_redraw = true; break;}
						case btn_select: { menu_mode_digit = 0; break;}
					}
				break;}
					
			}		
		}
