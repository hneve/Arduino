#define __DELAY_BACKWARD_COMPATIBLE__
#define 
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	unsigned char chSign,chEye;
	unsigned int iDelay;
	DDRD = 0xFF;                  // Set PORTD as Output
	chEye=0x01;                   // Initial Eye Variables with 0000 0001
	chSign=0;
	// Set ADCSRA Register in ATMega168
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
	// Set ADMUX Register in ATMega168
	ADMUX=0;

	for(;;) {                     // Loop Forever
		// Start conversion by setting ADSC in ADCSRA Register
		ADCSRA |= (1<<ADSC);
		// wait until conversion complete ADSC=0 -> Complete
		while (ADCSRA & (1<<ADSC));
		// Get ADC the Result
		iDelay = ADCW;
		if (iDelay < 1) iDelay=1;

		// Display the LED
		if (chSign == 0) {
			PORTD=chEye;
			_delay_ms(iDelay);          // Call Delay function
			chEye=chEye << 1;
			if (chEye >= 0x80) chSign=1;
			} else {
			PORTD=chEye;
			_delay_ms(iDelay);          // Call Delay function
			chEye=chEye >> 1;
			if (chEye <= 0x01) chSign=0;
		}
	}
	return 0;	                    // Standard Return Code
}