#ifndef ADC_LIBRARY
#define ADC_LIBRARY

#define ADC_PRESCALER_2		1
#define ADC_PRESCALER_4		2
#define ADC_PRESCALER_8		3
#define ADC_PRESCALER_16	4
#define ADC_PRESCALER_32	5
#define ADC_PRESCALER_64	6
#define ADC_PRESCALER_128	7

#define ADC_REF_ARef		0
#define ADC_REF_AVcc		1
#define ADC_REF_Int			3

void	adc_init(uint8_t, uint8_t);
uint8_t adc_8bit(uint8_t);
int		adc_10bit(uint8_t);
#endif