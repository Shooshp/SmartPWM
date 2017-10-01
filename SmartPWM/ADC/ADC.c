#include "ADC.h"

void ADC_INIT(void)
{
	ADC_DDR	 = 0x0; //ADC port as input
	ADC_PORT = 0x0; //No pull-ups

	ADMUX = 
	(
		(1<<REFS0)|
		(1<<REFS1) //internal 2.56V Voltage Reference with external capacitor at AREF pin
	);

	ADCSRA = 
	(
		(1<<ADEN) | // ADC Enable
		(1<<ADPS2)| // Prescaler of 128				
		(1<<ADPS1)| // 16000000/128 = 125000	
		(1<<ADPS0)
	);
}

uint16_t ADC_GET_VALUE(uint8_t CHANEL)
{	
	double RESULT = 0;
	uint16_t DATA =0;

	CHANEL &= 0x7; // of ‘CHANEL’ between 0 and 7
	ADMUX = (ADMUX & 0xF8)|CHANEL; // clears the bottom 3 bits before ORing

	_delay_us(10);

	for(uint8_t i=0; i<100; i++)
	{
		ADCSRA |= (1<<ADSC); // start single convertion
		while(ADCSRA & (1<<ADSC)); // wait for conversion to complete
	
		RESULT = RESULT + (VRef * ADC)/0x3FF;
	}

	RESULT /= 100;
	
	DATA = RESULT*1000;

	return DATA;
}

uint16_t GET_VOLTAGE(void)
{
	uint16_t VALUE = ADC_GET_VALUE(3) * 8;
	return VALUE;
}

uint16_t GET_CURRENT(void)
{
	uint16_t VALUE = ADC_GET_VALUE(2) * 2;
	return VALUE;
}