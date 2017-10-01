#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

#include "SPI/SPI.h"
#include "MCP4822/MCP4822.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "UTILITY.h"

unsigned char COMMAND = 0;
unsigned char ARGUMENT_LENGHT = 0;
unsigned char ARGUMENT[SIZE_RECEIVE_BUF-4];

unsigned char BUFFER[SIZE_RECEIVE_BUF];
unsigned char RECEIVING_TRANSMISSION = 0;
unsigned char DATA_RECEIVED = 0;
uint8_t DATA_BUFFER_COUNT = 0;

int main(void)
{
	MCP4822_INIT();
	ADC_INIT();
	UASRT_INIT();

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

	sei(); // Enable global interrupts
	
    while (1) 
    {	
		if (DATA_RECEIVED)
		{
			cli(); // Disable all global interrupts while working on command
			
			switch(COMMAND)
			{
				//TODO: need to prewrite basic commands and replays such as WhoIs? and Status
				case 0x1:
				break;				
			}
									
			DATA_RECEIVED = 0;
			sei(); // Enable global interrupts
		}
    }
}

uint16_t CHECK_CRC16 (uint8_t DATA_SIZE)
{
	uint16_t CRC = 0xFFFF;

	for (uint8_t counter = 0; counter < DATA_SIZE; counter++)
	{
		CRC = _crc16_update(CRC, BUFFER[counter]);
	}

	return CRC;
}

ISR (USART_RXC_vect)
{
	unsigned char RECEIVE_HIGH_BIT, RECEIVE_LOW_BYTE;
	
	while ( !(UCSRA & (1<<RXC)) );
	 
	RECEIVE_HIGH_BIT = RXB8;
	RECEIVE_LOW_BYTE = UDR;
	
	// Transmission was not established, received flag bit and address matches our own
	if ((RECEIVING_TRANSMISSION == 0)&&(RECEIVE_HIGH_BIT)&&(RECEIVE_LOW_BYTE == GET_ADDRESS()))
	{
		RECEIVING_TRANSMISSION = 1; // Set up for receiving
		DATA_RECEIVED = 0;          // Just in case clean receive completed flag
		DATA_BUFFER_COUNT = 0;      // Reset byte counter
	}
	
	// If transmission length exceeded maximum buffer size than in must be something went wrong, reset transmission.
	if (DATA_BUFFER_COUNT > SIZE_RECEIVE_BUF)
	{
		RECEIVING_TRANSMISSION = 0;
	} 
	else
	{
		// If it's not the end of transmission
		if (RECEIVE_LOW_BYTE != '\n')
		{
			BUFFER[DATA_BUFFER_COUNT] = RECEIVE_LOW_BYTE;
			DATA_BUFFER_COUNT++;
		} 
		else
		{
			// CRC Check was successful
			if (CHECK_CRC16(DATA_BUFFER_COUNT) == 0)
			{
				COMMAND         = BUFFER[1];
				ARGUMENT_LENGHT = BUFFER[2];
				
				for (uint8_t ARGUMENT_BYTE = 0; ARGUMENT_BYTE<ARGUMENT_LENGHT; ARGUMENT_BYTE++)
				{
					ARGUMENT[ARGUMENT_BYTE] = BUFFER[ARGUMENT_BYTE+3];
				}
				
				DATA_RECEIVED = 1;          // Set flag to signal main routine			
				//TODO: need to make reply function and use it here to signal RPi that receive was successful
			}
			else
			{
				// There was some errors in transmission so lets try again				
				DATA_RECEIVED = 0;          // Just in case clean receive completed flag				
			}
			RECEIVING_TRANSMISSION = 0; // Set up for receiving
			DATA_BUFFER_COUNT = 0;      // Reset byte counter
		}		
	}
}
