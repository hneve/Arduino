/*
 * LED_bar_8_cpp.cpp
 *
 * Created: 19.02.2014 11:24:32
 *  Author: Nivo
 */ 

#define F_CPU 16e6

void led_bar(int);
void led_light(int);

#include <avr/io.h>
#include <util/delay.h>

int number,U_D;
const int delaytime = 500;
int main(void){
//######################################## Setup	
	DDRD = 0xFF;
	U_D = 1;
//######################################## Loop 
    while(1){
// 		led_light(number);
// 		_delay_ms(delaytime);
		led_bar(number);
		_delay_ms(delaytime);
		if(U_D){
			number++;
		}
		else{
			number--;
		}
		if(number == 8) U_D = 0;
		if(number == 0) U_D = 1;
    }
}//######################################## End main

void led_bar(int level){
	PORTD = 0;
	while(level){
		PORTD = (PORTD << 1);
		PORTD++;
		level--;
	}
}

void led_light(int pin){
	PORTD = (1 << (pin-1));
	if(!pin) PORTD = 0;		
}