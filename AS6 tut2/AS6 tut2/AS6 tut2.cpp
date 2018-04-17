#define F_CPU 16e6
//////////////////////////////////////////////////////////////////////////
#define SETBIT(BYTEN, BIT) (BYTEN |= (1<<BIT))
#define CLRBIT(BYTEN, BIT) (BYTEN &= ~(1<<BIT))
#define FLPBIT(BYTEN, BIT) (BYTEN ^= (1<<BIT))
#define CHKBIT(BYTEN, BIT) (BYTEN & (1<<BIT))
//////////////////////////////////////////////////////////////////////////
#define LED PORTB5
#define KNAPP PORTB4
#define ER_KNAPP_LAV !CHKBIT(PINB,4)
//////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	SETBIT(DDRB,LED);		//LED = PORTB5 som utgang
	SETBIT(PINB,KNAPP);		//KNAPP = PORTB4 slå på pull-up
	while(1)
	{
		if(ER_KNAPP_LAV){
			FLPBIT(PORTB,PORTB5);
			_delay_ms(1000);
		}
	}
}