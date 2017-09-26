#include "UTILITY.h"
#include "USART/USART.h"

void SET_CURRENT(int VALUE)
{
	if(VALUE > 3000)
	{
		VALUE = 3000;
	}

	VALUE = VALUE * 1.363;

	MCP4822_SET(1, VALUE);
}

void SET_VOLTAGE(int VALUE)
{
	int DATA = VALUE;

	if (DATA > 20000)
	{
		DATA = 20000;
	}

	DATA = DATA / 4.97;


	MCP4822_SET(0, DATA);
}

void POWER_ON (void)
{
	UART_PORT &= ~(1 << PWR_CTR_PIN);
	_delay_ms(10);
	UART_PORT |=  (1 << PRT_RST_PIN);
	_delay_ms(100);
	UART_PORT &= ~(1 << PRT_RST_PIN);
};

void POWER_OFF (void)
{
	UART_PORT |=  (1 << PWR_CTR_PIN);
	_delay_ms(10);
};