int adc_8bit(int adpin){
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
	ADMUX |= _BV(ADLAR) | _BV(REFS0);
	ADMUX |= adpin;
	ADCSRA |= _BV(ADSC);
	while (ADCSRA & _BV(ADSC));
	return ADCH;
}