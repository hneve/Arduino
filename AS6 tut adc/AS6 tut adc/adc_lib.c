/*
Atmel Studio Library

ADC functions

adc_8bit( analog pin to use ) 
returns 8bit integer

adc_10bit( analog pin to use )
returns 10bit integer

Defaults:
adc clock div 64
adc ref AVcc

*/

//#include <avr/io.h>
#include "adc_lib.h"

void adc_init(uint8_t ADC_PRESCALER, uint8_t ADC_REF ){
	// Clear ADCSRA, set ADEN and the ADC_PRESCALER
	ADCSRA = (1 << ADEN) | ADC_PRESCALER;
	// Clear ADMUX and set ADC_REF
	ADMUX = (ADC_REF << 6);
}

uint8_t adc_8bit(uint8_t apin){
	ADMUX |= (1 << ADLAR); // 8bit
	ADMUX &= 0xF0; // clear what pin used.
	ADMUX |= apin; // set pin used.

	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADCH;
}

int adc_10bit(uint8_t apin){
	ADMUX &= ~(1 << ADLAR); // 10bit
	ADMUX &= 0xF0; // clear what pin used.
	ADMUX |= apin; // set pin used.

	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADCW; // Return whole ADCH + ADCL
}