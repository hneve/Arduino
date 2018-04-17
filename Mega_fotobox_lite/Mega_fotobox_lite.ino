//DEFINE bit manipulation:
#define setbit(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define clearbit(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define flipbit(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define checkbit(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define setbitmask(x,y) (x |= (y))
#define clearbitmask(x,y) (x &= (~y))
#define flipbitmask(x,y) (x ^= (y))
#define checkbitmask(x,y) (x & (y))
//DEFINE IO PINS
#define Button			checkbit(PINH,5)
#define Dsensor			checkbit(PINF,1)

#define Shutter_open	setbit(PORTB,7)
#define Shutter_close	clearbit(PORTB,7)
#define Focus_open		setbit(PORTB,6)
#define Focus_close		clearbit(PORTB,6)
#define Flash_open		setbit(PORTB,5)
#define Flash_close		clearbit(PORTB,5)
#define LED_red_on		setbit(PORTB,4)
#define LED_red_off		clearbit(PORTB,4)
#define LED_grn_on		setbit(PORTH,6)
#define LED_grn_off		clearbit(PORTH,6)

#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

unsigned int tdelay = 0;
unsigned int tdelay_new;
unsigned long timestamp; 	
void setup() { 
	lcd.begin(16, 2);  
	setbit(DDRB,7);		//Shutter output
	setbit(DDRB,6);		//Focus output
	setbit(DDRB,5);		//Flash output
	setbit(DDRB,4);		//LED output
	setbit(DDRH,6);		//LED output
	setbit(PORTH,5);	//internal pull up for button

	
	LED_grn_on;
}
void loop (){
	if(millis() >= timestamp){
		int tdelay_new =map(analogRead(A2),0,1023,10,0);
		lcd.clear();
		lcd.print(tdelay_new);
		lcd.print(" ms");
		if (tdelay != tdelay_new ){
			tdelay = tdelay_new;
		}
	  timestamp = millis() + 1000;
	}

	
	if (!Button){
		
		delay(1000);
		Focus_open;
		Shutter_open;
		LED_grn_off;
		LED_red_on; 
		bool old_Dsensor = Dsensor;
		while(Dsensor == old_Dsensor){}
		
		LED_grn_on;
		delay(tdelay);
		LED_red_off;
		Flash_open;
		delay(50);
		Flash_close;
		Shutter_close;
		Focus_close;
	}
}
