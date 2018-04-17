#include <avr/io.h>
#include "adc_lib.c"

int main(void)
{
	adc_init(ADC_PRESCALER_64,ADC_REF_AVcc);
	DDRD = 0xFF;
 	while(1)
	 {
 		PORTD = adc_8bit(0);
     }
}