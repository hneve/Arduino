#define F_CPU 16e6
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB =  0b00100000;
	PORTB = 0b00010000;
    while(1)
    {
		if(!(PINB & 0b00010000)){
			PORTB = 0b00110000;
			_delay_ms(500);
			PORTB = 0b00010000;
			_delay_ms(1000);		
		}
    }
}