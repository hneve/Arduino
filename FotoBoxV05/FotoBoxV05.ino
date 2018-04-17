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

//DEFINE bit manipulation:

#define setbit(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define clearbit(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define flipbit(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define checkbit(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define setbitmask(x,y) (x |= (y))
#define clearbitmask(x,y) (x &= (~y))
#define flipbitmask(x,y) (x ^= (y))
#define checkbitmask(x,y) (x & (y))
//Define in/outputs bitwize
#define flash		flipbit(PORTC,2)
#define shutter		flipbit(PORTC,3)
#define focus		flipbit(PORTC,4)
#define external	flipbit(PORTC,5)	
#define dsensor		checkbit(PINC,0)

#define btn1	checkbit(PIND,0)
#define btn2	checkbit(PIND,1)
#define btn3	checkbit(PIND,2)
#define btn4	checkbit(PIND,3)

//Define pins and values:
#define pin_sensor_a		A0
#define pin_sensor_d		14
#define port_flash2			10 
#define port_flash			11
#define port_cc_focus		12
#define cc_trigger_port		13
#define std_backlight_value 128
//Includes and starts:
#include <LiquidCrystal.h>
//#include <key_read.h>
//#include <IRremote.h>
LiquidCrystal lcd(9,8,7,6,5,4);	// Setup LiquidCrystal
//key_read kb(0,769);						// Setup key_read: Analog pin 0, lcd blinks at 1/1.3 sec = .769 seconds
//int RECV_PIN = 2;
//IRrecv irrecv(RECV_PIN);
//decode_results results;

char	lcdline1[20];
char	lcdline2[20];
int		analog_level;
int		analog_level_high;
int		analog_level_low;
int		analog_level_threshold = 10;
char	digital_level;

bool	val_up;
bool	val_down;
bool	sel_btn;
bool	analog_sensor = true;
int		delaytime[2];
int		delay_inc = 1;
int		drops = 1;
int		dropsize = 10;
int		mode = 0; 
int		menu = 0;
int		end_menu = 6;
int		menuitem=0;
int		menuline[4][7] = {{0,1,2,6,0,0,0},{0,1,2,6,0,0,0},{0,2,6,0,0,0,0},{0,1,2,3,4,5,6}};

void setup()
{
ADCSRA |=  (1 << ADPS2);		// ADC prescaler 16
setbitmask(DDRB,0b00111100);	//set portB 5,4,3,2 to outputs for optocoupler
//setbitmask(DDRC,0b00000000);
setbitmask(PORTD, 0b00001111);	//Internal pullups for buttons
//analogWrite(lcd_backlight_pwm,std_backlight_value);  
lcd.begin(16,2);
lcd.print("FotoBox v.0.5");
delay(500);
lcd.clear();
//irrecv.enableIRIn(); // Start the receiver
}
void loop(){
switch(menu){
	case 0:{set_mode();break;}	
	case 1:{sensor_setup();break;}
	case 2:{set_delay(0);break;}
	case 3:{set_delay(1);break;}	
	case 4:{set_drops();break;} //set drops
	case 5:{set_dropsize();break;} // drop size
	case 6:{RUN();break;} // run
}// END switch menu
update_lcd();
get_buttons();

}// END LOOP
void get_buttons(){
	

/* LCD SHIELD analog buttons 
	switch (kb.read()) {
		//case btn_right: menu++;menu=constrain(menu,0,end_menu);lcd.clear(); break;	
		case btn_right: if(menu < 6) {menuitem++; menu = menuline[mode][menuitem];lcd.clear();} break;			
		//case btn_left:	menu--;menu=constrain(menu,0,end_menu);lcd.clear(); break;					
		case btn_left:	if(menu) {menuitem--; menu = menuline[mode][menuitem];lcd.clear();} break;	
		case btn_up:	val_up=true;lcd.clear();break;
		case btn_down:	val_down=true;lcd.clear();break;
		case btn_select:sel_btn=true;lcd.clear();break;
	} 
*/


if (!btn1) { if(menu < 6) {menuitem++; menu = menuline[mode][menuitem];lcd.clear();} }
if (!btn2) {if(menu) {menuitem--; menu = menuline[mode][menuitem];lcd.clear();}}
if (!btn3) {val_up=true;lcd.clear();}
if (!btn4) {val_down=true;lcd.clear();}
//delay(100);

/*  IR library	
	if (irrecv.decode(&results)) {
		switch (results.value){
			case 0x77E1D035: val_up=true;lcd.clear();break;
			case 0x77E1B035: val_down=true;lcd.clear();break;
			case 0x77E11035: if(menu) {menuitem--; menu = menuline[mode][menuitem];lcd.clear();} break;
			case 0x77E1E035: if(menu < 6) {menuitem++; menu = menuline[mode][menuitem];lcd.clear();} break;
			case 0x77E12035: sel_btn=true;lcd.clear();break;
			case 0x77E14035:break; //menu
			case 0xFFFFFFFF:break; //repeat
		}
		//Serial.println(results.value, HEX);
		
		irrecv.resume(); // Receive the next value
		
	} */
}
void update_lcd(){
	lcd.setCursor(0,0);
	if(menu) lcd.print("<"); else lcd.print("#");
	lcd.setCursor(15,0);
	if(menu == end_menu) lcd.print("#"); else lcd.print(">");
	lcd.setCursor(2,0);
	lcd.print(lcdline1);
	lcd.setCursor(0,1);
	lcd.print(lcdline2);
}
void set_mode(){
	
	if(val_up)		 mode++;val_up=false;
	if(val_down)	 mode--;val_down=false;
	mode = constrain(mode,0,3);
	
	sprintf(lcdline1,"MODE");
	if(mode == 0) sprintf(lcdline2,"Flashtrigger");	
	if(mode == 1) sprintf(lcdline2,"Cameratrigger");
	if(mode == 2) sprintf(lcdline2,"Timelapse");	
	if(mode == 3) sprintf(lcdline2,"Ext Control");	
}
void sensor_setup(){
	if(val_up)		analog_sensor = true;	val_up = false;
	if(val_down)	analog_sensor = false;  val_down = false;
	if(sel_btn){
		switch(analog_level_threshold){
			case 1:	analog_level_threshold = 10;break;
			case 10: analog_level_threshold = 100;break;
			case 100: analog_level_threshold = 1;break;
		}
		sel_btn = false;
	}
	sprintf(lcdline1,"SENSOR SETUP");
	
	if(analog_sensor){
		analog_level = analogRead(pin_sensor_a);
		sprintf(lcdline2, "A:%4i   T:%3i", analog_level,analog_level_threshold);
	}
	else{
		if(dsensor) digital_level = 'H'; else digital_level = 'L';
		sprintf(lcdline2, "D:%c", digital_level);
	}
}
void rot_del_inc(){
	switch(delay_inc){
		case 1:	delay_inc = 10;break;
		case 10: delay_inc = 100;break;
		case 100: delay_inc = 1000;break;
		case 1000: delay_inc = 1;break;
	}
}
void set_delay(int nr){
	if(val_up  ) delaytime[nr]+=delay_inc;delaytime[nr]=constrain(delaytime[nr],0,9999);val_up = false;
	if(val_down) delaytime[nr]-=delay_inc;delaytime[nr]=constrain(delaytime[nr],0,9999);val_down = false;
	if(sel_btn)  rot_del_inc();sel_btn = false;
	
	sprintf(lcdline1,"Delay%i x%4i",nr,delay_inc);
	sprintf(lcdline2,"%4i",delaytime[nr]);
}
void set_drops(){
	if(val_up) drops++;drops = constrain(drops,1,10);val_up = false;
	if(val_down) drops--;drops = constrain(drops,1,10);val_down = false;
	sprintf(lcdline1,"Drops 1-10");
	sprintf(lcdline2,"%4i",drops);	
}
void set_dropsize(){
	if(val_up) dropsize++;dropsize = constrain(dropsize,1,100);val_up = false;
	if(val_down) dropsize--;dropsize = constrain(dropsize,1,100);val_down = false;
	sprintf(lcdline1,"Drop size");
	sprintf(lcdline2,"%4i",dropsize);
}
void RUN(){
	sprintf(lcdline1,"RUN?");
	sprintf(lcdline2,"");
	
	if(sel_btn){
		sel_btn = false;
		
		switch (mode){
			case 0:	if(analog_sensor){RUN_analog_flashtrigger();}
					else{RUN_digital_flashtrigger();}
					break;
			case 1: if(analog_sensor){RUN_analog_cameratrigger();}
					else{RUN_digital_cameratrigger();}
					break;
			case 2: RUN_timelapse();
					break;
			
		}
	}
}
void calibrate(int time){
	// Calibrate analog input to create high and low trigger level
	lcd.clear();
	time *= 1000;
	lcd.setCursor(0,0);
	lcd.print("Calibrating ");

	int level = analogRead(pin_sensor_a);
	analog_level_low = level-1;
	analog_level_high = level+1;
	unsigned long timestamp = millis() + time;
	while (millis() < timestamp) {
		level = analogRead(pin_sensor_a);
		if (level < analog_level_low) { analog_level_low = level;}
		if (level > analog_level_high) { analog_level_high = level;}
		lcd.setCursor(13,0);
		lcd.print((timestamp - millis()) / 1000);
		lcd.setCursor(0,1);
		lcd.print("H");
		lcd.print(analog_level_high);
		lcd.print(" L");
		lcd.print(analog_level_low);
	}
	analog_level_low -= analog_level_threshold;
	analog_level_high += analog_level_threshold;
	lcd.clear();
}
void RUN_analog_flashtrigger(){
	calibrate(5);
	//analogWrite(lcd_backlight_pwm, 0);     // Turn off backlight LCD
	focus;
	shutter;
	analog_level = analogRead(pin_sensor_a);
	
		while ((analog_level > analog_level_low) && (analog_level < analog_level_high)) { analog_level = analogRead(pin_sensor_a); }
			
	delay(delaytime[0]);                     
	flash;
	delay(10);
	flash;
	shutter;
	focus;
	//analogWrite(lcd_backlight_pwm, std_backlight_value);
}	
void RUN_digital_flashtrigger(){
	//analogWrite(lcd_backlight_pwm,0);	
	focus;	//focus ON
	shutter; // Shutter ON
	bool	dsensor_last = dsensor;
	
		while(dsensor == dsensor_last) {}
			
	delay(delaytime[0]);
	flash; //Flash ON
	delay(10);
	flash; // Flash OFF
	focus; // OFF
	shutter; // OFF
		
	//analogWrite(lcd_backlight_pwm,std_backlight_value);	
}
void RUN_analog_cameratrigger(){
	calibrate(5);
	//analogWrite(lcd_backlight_pwm, 0);     // Turn off backlight LCD
	analog_level = analogRead(pin_sensor_a);
	
	while ((analog_level > analog_level_low) && (analog_level < analog_level_high)) { analog_level = analogRead(pin_sensor_a); }
		
	delay(delaytime[0]);
	shutter;
	delay(100);
	shutter;
	//analogWrite(lcd_backlight_pwm, std_backlight_value);
}
void RUN_digital_cameratrigger(){
	//analogWrite(lcd_backlight_pwm,0);
	
	bool	dsensor_last = dsensor;
	
	while(dsensor == dsensor_last) {}
	
	delay(delaytime[0]);
	focus;
	shutter; // Shutter ON
	delay(800);
	shutter; // OFF
	focus;
	//analogWrite(lcd_backlight_pwm,std_backlight_value);
}
void RUN_timelapse(){
	//analogWrite(lcd_backlight_pwm,0);
	while(kb.read() == btn_none){
	delay(delaytime[0] * 1000);
	focus;
	shutter; // Shutter ON
	delay(200);
	shutter; // OFF
	focus;
	}
	//analogWrite(lcd_backlight_pwm,std_backlight_value);
}