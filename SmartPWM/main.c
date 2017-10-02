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
unsigned char MAX_LENGHT = 0;
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

uint8_t CHECK_CRC16 (void)
{
	uint16_t CRC_CALC = 0x0;
	uint16_t CRC_GET = (BUFFER[DATA_BUFFER_COUNT-1])|(BUFFER[DATA_BUFFER_COUNT-2]<<8);

	for (uint8_t counter = 0; counter < (DATA_BUFFER_COUNT - 2); counter++)
	{
		CRC_CALC = _crc16_update(CRC_CALC, BUFFER[counter]);
	}
	
	if (CRC_CALC == CRC_GET)
	{
		return 1;	
	} 
	else
	{
		return 0;
	}
}

void SEND_ACKNOWLEDGE (void)
{
	BUFFER[0] = GET_ADDRESS();				 // Our own address
	BUFFER[1] = BUFFER[DATA_BUFFER_COUNT-2]; // First CRC16 Byte
	BUFFER[2] = BUFFER[DATA_BUFFER_COUNT-1]; // Second CRC16 Byte

	RS485_WRITE();
	for(uint8_t SEND_BYTE_COUNT = 0; SEND_BYTE_COUNT<3; SEND_BYTE_COUNT++)
	{
		USART_PUTCHAR(BUFFER[SEND_BYTE_COUNT]);
	}
	RS485_READ();
}

ISR (USART_RXC_vect)
{
	unsigned char RECEIVE_BYTE;
	 
	RECEIVE_BYTE = USART_GETCHAR();
	
	// Transmission was not established, received flag bit and address matches our own
	if ((RECEIVING_TRANSMISSION == 0)&&(RECEIVE_BYTE == GET_ADDRESS()))
	{
		RECEIVING_TRANSMISSION = 1; // Set up for receiving
		DATA_RECEIVED = 0;          // Just in case clean receive completed flag
		DATA_BUFFER_COUNT = 0;      // Reset byte counter
		MAX_LENGHT = 4;             // By that byte we should get data lenght
	}
	
	if (RECEIVING_TRANSMISSION)
	{
		// If transmission length exceeded maximum buffer size 
		// than in must be something went wrong, reset transmission.
		if (DATA_BUFFER_COUNT > SIZE_RECEIVE_BUF)
		{
			RECEIVING_TRANSMISSION = 0;
		} 
		else
		{
			if (DATA_BUFFER_COUNT == 2)
			{
				// Full packet length = address[1] + command[1] + length[1] +
				// + data[length] + CRC[2] = length + 5
				MAX_LENGHT = RECEIVE_BYTE + 5; 
			}
			// If it's not the end of transmission
			if (DATA_BUFFER_COUNT < MAX_LENGHT)
			{
				BUFFER[DATA_BUFFER_COUNT] = RECEIVE_BYTE;
				DATA_BUFFER_COUNT++;
			} 
			else
			{
				// CRC Check was successful
				if (CHECK_CRC16())
				{
					COMMAND         = BUFFER[1];
					ARGUMENT_LENGHT = BUFFER[2];
				
					for (uint8_t ARGUMENT_BYTE = 0; ARGUMENT_BYTE<ARGUMENT_LENGHT; ARGUMENT_BYTE++)
					{
						ARGUMENT[ARGUMENT_BYTE] = BUFFER[ARGUMENT_BYTE+3];
					}
				
					SEND_ACKNOWLEDGE();         // Signal to RPi that data was successfully received
					DATA_RECEIVED = 1;          // Set flag to signal main routine							
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
}
