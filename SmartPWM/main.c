#define	F_CPU (16000000UL)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/crc16.h>

#include "SPI/SPI.h"
#include "MCP4822/MCP4822.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "PARSER/PARSER.h"

unsigned char BUFFER[SIZE_RECEIVE_BUF];
volatile unsigned char COMMAND[32];
volatile int ARGUMENT;
char RECIVE_FLAG;
uint8_t data_count = 0;

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

ISR (USART_RXC_vect)
{
	if ((data_count != 0)||(UDR == GET_ADDRESS()))
	{
		if (data_count == 0)
		{
			RECIVE_FLAG = 0;
		}

		BUFFER[data_count] = UDR;
		data_count++;

		if (BUFFER[data_count] == '\n')
		{
			if (CHECK_CRC16(data_count)==0)
			{
				RECIVE_FLAG = 1;
			}
		}
	}
}

uint16_t CHECK_CRC16 (uint8_t DATA_SIZE)
{
	uint8_t counter;
	uint16_t CRC = 0xFFFF;

	for (counter = 0; counter < DATA_SIZE; counter++)
	{
		CRC = _crc16_update(CRC, BUFFER[counter]);
	}

	return CRC;
}

uint8_t GET_ADDRESS (void)
{
	uint8_t DEVICE_ADDRESS;

	DEVICE_ADDRESS = ADDRESS_PORT & 0xF0;

	return DEVICE_ADDRESS;
}
