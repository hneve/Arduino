// Project Fotobox
// Auth: Hans . Neve (at) gmail.com
// www.neve.nu
//
#define VERSION	"FotoBox v.0.6"
//
// High speed flash trigger
// Camera trigger - function
// Timelapse - function


// TOP SECTION .
//DEFINE bit manipulation:
#define setbit(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define clearbit(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define flipbit(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define checkbit(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define setbitmask(x,y) (x |= (y))
#define clearbitmask(x,y) (x &= (~y))
#define flipbitmask(x,y) (x ^= (y))
#define checkbitmask(x,y) (x & (y))

//Define outputs
#define shutter		flipbit(PORTB,5) //13
#define focus		flipbit(PORTB,4) //12
#define flash		flipbit(PORTB,3) //11
#define external	flipbit(PORTB,2) //10

//Define inputs
#define dsensor		checkbit(PINC,0)	// A0-aka-14
#define button1		!checkbit(PIND,0)	// All buttons are active low - thus inverted.
#define button2		!checkbit(PIND,1)
#define button3		!checkbit(PIND,2)
#define button4		!checkbit(PIND,3)
#define asensor		A0

//Includes and variables:
#include <LiquidCrystal.h>
LiquidCrystal lcd(9,8,7,6,5,4);	// Setup LiquidCrystal

char		lcdline1[20],lcdline2[20];
char 		digital_level;
int			analog_level,analog_level_high,analog_level_low,analog_level_threshold = 10;
int			delaytime[2];
int			delay_inc = 1;
int			drops = 1;
int			dropsize = 10;
int			mode = 0;
int			menu = 0;
int			end_menu = 6;
int			menuitem=0;
int			menuline[4][7] = {{0,1,2,6,0,0,0},{0,1,2,6,0,0,0},{0,2,6,0,0,0,0},{0,1,2,3,4,5,6}};
int			button_repeat_speed;
int			button_held;
bool		button_plus,button_minus;
bool		button_right,button_left;
bool		analog_sensor = true;
long		button_count;
byte		button;
byte		old_button;

void setup(){
	ADCSRA |=  (1 << ADPS2);		// ADC prescaler 16
	setbitmask(DDRB,0b00111100);	//set portB 5,4,3,2 to outputs for optocoupler
	setbitmask(PORTD, 0b00001111);	//Internal pullups for buttons
	lcd.begin(16,2);
	lcd.print(VERSION);
	delay(1500);
	lcd.clear();
	if(button4) SystemTest();
}
void loop(){
	switch(menu){
		case 0:{set_mode();break;}
		case 1:{sensor_setup();break;}
		case 2:{set_delay(0);break;}
		case 3:{set_delay(1);break;}
//		case 4:{set_drops();break;}
//		case 5:{set_dropsize();break;}
		case 6:{RUN();break;}
	}
	update_lcd();
	get_buttons();
}
void get_buttons(){
	button = ((button4 << 3) | (button3 << 2) | (button2 << 1) | (button1 << 0));
	if (button == old_button){
		button_held++;
		if(millis() <= (button_count + button_repeat_speed)) {return;}
	}
	
	switch (button){
		case 1:
		button_right = true;
		button_repeat_speed = 500;
		break;
		case 2:
		button_left = true;
		button_repeat_speed = 500;
		break;
		case 4:
		button_plus=true;
		if(button_held > 100) button_repeat_speed = 50;
		break;
		case 8:
		button_minus=true;
		if(button_held > 100) button_repeat_speed = 50;
		break;
		default:
		button_held =0;
		button_repeat_speed = 500;
		break;
	}
	old_button = button;
	button_count = millis();
	
}
void update_lcd(){
	lcd.setCursor(0,0);
	if(menu) lcd.print("<");
	else lcd.print("#");
	lcd.setCursor(15,0);
	if(menu == end_menu) lcd.print("#");
	else lcd.print(">");
	lcd.setCursor(2,0);
	lcd.print(lcdline1);
	lcd.setCursor(0,1);
	lcd.print(lcdline2);
}
void set_mode(){
	if(button_plus) {
		mode++;
		button_plus=false;
	}
	if(button_minus) {
		mode--;
		button_minus=false;
	}
	if(button_right) {
		menu = menuline[mode][++menuitem];
		button_right=false;
	}
	if(button_left) button_left=false;
	mode = constrain(mode,0,2);
	sprintf(lcdline1,"MODE        ");
	if(mode == 0) sprintf(lcdline2,"Flashtrigger    ");
	if(mode == 1) sprintf(lcdline2,"Cameratrigger   ");
	if(mode == 2) sprintf(lcdline2,"Timelapse       ");
	//if(mode == 3) sprintf(lcdline2,"Ext Control     ");
}
void sensor_setup(){
	if(button_plus & !analog_sensor){	
		analog_sensor = true;
		button_plus = false;
	}
	if(button_plus & analog_sensor){
		switch(analog_level_threshold){
			case 1:	analog_level_threshold = 10;break;
			case 10: analog_level_threshold = 100;break;
			case 100: analog_level_threshold = 1;break;
		}
		button_plus = false;
	}
	if(button_minus){
		analog_sensor = false;  
		button_minus = false;
	}
	if(button_right){
		menu = menuline[mode][++menuitem];
		button_right=false;
	}
	if(button_left){
		menu = menuline[mode][--menuitem];
		button_left=false;
	}
	sprintf(lcdline1,"SENSOR SETUP");
	if(analog_sensor){
		analog_level = analogRead(asensor);
		sprintf(lcdline2, "A:%4i   T:%3i", analog_level,analog_level_threshold);
	}
	else{
		if(dsensor) digital_level = 'H'; else digital_level = 'L';
		sprintf(lcdline2, "D:%c             ", digital_level);
	}
}
void set_delay(int nr){
	
	if(button_plus ){
		delaytime[nr]+=delay_inc;
		delaytime[nr]=constrain(delaytime[nr],0,9999);
		button_plus  = false;
	}
	if(button_minus){
		delaytime[nr]-=delay_inc;
		delaytime[nr]=constrain(delaytime[nr],0,9999);
		button_minus = false;
	}
	if(button_right){
		menu = menuline[mode][++menuitem];
		button_right = false;
	}
	if(button_left ){
		menu = menuline[mode][--menuitem];
		button_left  = false;
	}
	sprintf(lcdline1,"DELAY TIME  ",nr,delay_inc);
	sprintf(lcdline2,"%4i            ",delaytime[nr]);
}
// void set_drops(){
// 	if(button_plus){ 
// 		drops++;
// 		drops = constrain(drops,1,10);
// 		button_plus = false;
// 	if(button_minus) drops--;drops = constrain(drops,1,10);button_minus = false;
// 	if(button_right) menu = menuline[mode][++menuitem];button_right=false;
// 	if(button_left) menu = menuline[mode][--menuitem];button_left=false;
// 	sprintf(lcdline1,"Drops 1-10");
// 	sprintf(lcdline2,"%4i",drops);
// }
// void set_dropsize(){
// 	if(button_plus) dropsize++;dropsize = constrain(dropsize,1,100);button_plus = false;
// 	if(button_minus) dropsize--;dropsize = constrain(dropsize,1,100);button_minus = false;
// 	if(button_right) menu = menuline[mode][++menuitem];button_right=false;
// 	if(button_left) menu = menuline[mode][--menuitem];button_left=false;
// 	sprintf(lcdline1,"Drop size");
// 	sprintf(lcdline2,"%4i",dropsize);
// }
void RUN(){
	sprintf(lcdline1,"RUN?        ");
	sprintf(lcdline2,"            ");
	
	if(button_plus){
		button_plus = false;
		
		switch (mode){
			case 0:	
				if(analog_sensor){RUN_analog_flashtrigger();}
				else{RUN_digital_flashtrigger();}
				break;
			case 1: 
				if(analog_sensor){RUN_analog_cameratrigger();}
				else{RUN_digital_cameratrigger();}
				break;
			case 2: 
				RUN_timelapse();
				break;
			
		}
	}
	if(button_left){ 
		menu = menuline[mode][--menuitem];
		button_left=false;
	}	
	if(button_right) button_right=false;
}
void calibrate(int time){
	// Calibrate analog input to create high and low trigger level
	lcd.clear();
	time *= 1000;
	lcd.print("Calibrating");

	int level = analogRead(asensor);
	analog_level_low = level-1;
	analog_level_high = level+1;
	unsigned long timestamp = millis() + time;
	while (millis() < timestamp) {
		level = analogRead(asensor);
		if (level < analog_level_low)	{ analog_level_low  = level;}
		if (level > analog_level_high)	{ analog_level_high = level;}
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
	PrintRunning();
	focus;
	shutter;
	analog_level = analogRead(asensor);
	while (((analog_level > analog_level_low) & (analog_level < analog_level_high)) & !button4 ) {
		analog_level = analogRead(asensor);
	}
	delay(delaytime[0]);
	flash;
	external;
	delay(10);
	flash;
	external;
	shutter;
	focus;
}
void RUN_digital_flashtrigger(){
	PrintRunning();
	focus;
	shutter;
	bool dsensor_last = dsensor;
	while((dsensor == dsensor_last) & !button4) {}
	delay(delaytime[0]);
	flash;
	external;
	delay(10);
	flash;
	external;
	focus;
	shutter;
}
void RUN_analog_cameratrigger(){
	calibrate(5);
	PrintRunning();
	analog_level = analogRead(asensor);
	while (((analog_level > analog_level_low) && (analog_level < analog_level_high)) & !button4) {
		analog_level = analogRead(asensor);
	}
	delay(delaytime[0]);
	focus;
	shutter;
	delay(15);
	flash;
	delay(85);
	shutter;
	focus;
	flash;
}
void RUN_digital_cameratrigger(){
	PrintRunning();
	bool dsensor_last = dsensor;
	while((dsensor == dsensor_last) & !button4) {}
	delay(delaytime[0]);
	focus;
	shutter;
	delay(15);
	flash;
	delay(85);
	shutter;
	focus;
	flash;
}
void RUN_timelapse(){
	int tl_delay = delaytime[0];
	if (tl_delay == 0) tl_delay = 500;	
	PrintRunning();
	static long timelapse_last;
	while(!button4){
		if(millis() > timelapse_last + (tl_delay) ){
			focus;
			shutter;
			delay(200);
			shutter;
			focus;
			timelapse_last = millis();
		}
	}
}
void PrintRunning(){
	lcd.clear();
	lcd.print("  Running!");
}
void SystemTest(){
	lcd.print("System testing:");
	lcd.setCursor(0,1);
	lcd.print("1f 2s 3-1 4-2");
        bool hold = 1;
	while(hold){
		get_buttons();
		switch (button){
			case 1: shutter;break;
			case 2: focus;break;
			case 4: flash;break;
			case 8: external;break;
                        case 15: hold = 0;break;
		}
		
	}
}
