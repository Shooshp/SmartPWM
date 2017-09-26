#include "PARSER.h"

#define TRUE   1
#define FALSE  0

unsigned char BUFFER[SIZE_RECEIVE_BUF];

uint8_t data_count = 0;

void PARSER_INIT(void)
{
	ADDRESS_DDR  &= ~((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3));
	ADDRESS_PORT &= ~((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3));
}

uint8_t GET_ADDRESS (void)
{
	uint8_t DEVICE_ADDRESS;

	DEVICE_ADDRESS = ADDRESS_PORT & 0xF0;

	return DEVICE_ADDRESS;
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
