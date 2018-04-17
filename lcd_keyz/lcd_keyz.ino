#include <Arduino.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <key_read.h>
key_read kb(0,100);

#define lcd_bg_pwn	10


int hor;
int vert;
char buffer[16];

void setup()
{
lcd.begin(16, 2);
lcd.blink();


}

void loop()
{
	switch (kb.read()){
		case btn_up: vert--; if(vert < 0) vert = 0;
		break;
		case btn_down: vert++; if(vert > 1) vert = 1;
		break;
		case btn_right: hor++; if (hor > 15) hor = 15;
		break;
		case btn_left: hor--; if (hor < 0) hor = 0;
		break;
		case btn_select: hor=0; vert=0;
		break;
		case 5:
		break;

		
	}
// in = kb.read();
 lcd.setCursor(hor,vert);
 //lcd.print(in);
// lcd.setCursor(0,1);
// lcd.print(analogRead(0));
// lcd.print("             ");

}
