#define	F_CPU (16000000UL)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

volatile unsigned char COMMAND[32];
volatile int ARGUMENT;
char RECIVE_FLAG;

#include "SPI/SPI.h"
#include "MCP4822/MCP4822.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "PARSER/PARSER.h"



int main(void)
{

	MCP4822_INIT();
	ADC_INIT();
	UASRT_INIT();
	PARSER_INIT();

	// Заглушка для ОВ протекшена и его сброса
	DDRC  |= (1<<PINC7);
	PORTC &=~(1<<PINC7);
	
	// Гипотетическое прерывание от ОВ протекшена, в макете он отключен так что хз работает ли это вообще.

	/*
	
	INT0_DDR &=~(1<<INT0_PIN); // Настройка PD0 вход для получения прерывания на этот пин
	GICR = 1<<INT0;            // Включение внешнего прерывания

	// по сути ISC11 = 0 и ISC10 что даёт "The low level of INT1 generates an interrupt request."
	MCUCR  &=
	~(
		(1 << ISC01)|
		(1 << ISC00)
	);
	*/

	sei();
	
    while (1) 
    {
    }
}

