#include "UTILITY.h"

void SET_CURRENT(uint16_t VALUE)
{
	if (VALUE > 3000)
		VALUE = 3000;

	VALUE = VALUE * 1.363;
	MCP4822_SET(1, VALUE);
}

void SET_VOLTAGE(uint16_t VALUE)
{
	if (VALUE > 20000)
		VALUE = 20000;

	VALUE = VALUE / 4.97;
	MCP4822_SET(0, VALUE);
}

void POWER_ON (void)
{
	UART_PORT &= ~(1 << PWR_CTR_PIN);
	_delay_ms(10);
	UART_PORT |=  (1 << PRT_RST_PIN);
	_delay_ms(100);
	UART_PORT &= ~(1 << PRT_RST_PIN);
}

void POWER_OFF (void)
{
	UART_PORT |=  (1 << PWR_CTR_PIN);
	_delay_ms(10);
}

uint8_t GET_ADDRESS (void)
{
	uint8_t DEVICE_ADDRESS = (ADDRESS_PIN & 0xF)|POWER_SOURCE_PREFIX;
	return DEVICE_ADDRESS;
}