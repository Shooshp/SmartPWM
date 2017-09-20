#include "MCP4822.h"

void MCP4822_INIT(void)
{
	SPI_INIT();
	MCP4822_DDR  |= (1 << LDAC);
	MCP4822_PORT |= (1 << LDAC);

	MCP4822_SET(0,0);
	MCP4822_SET(1,0);
}

void MCP4822_SET(uint8_t CHANEL, uint16_t DATA)
{
	if (CHANEL)
	{
		DATA = DATA | 0x9000;
	} 
	else
	{
		DATA = DATA | 0x1000;
	}

	SPI_SELECT();
	_delay_us(100);

	SPI_WRITE_BYTE(0xFF & (DATA>>8));
	SPI_WRITE_BYTE(0xFF &  DATA);

	SPI_DESELECT();

	MCP4822_PORT &= ~(1 << LDAC);
	_delay_us(100);
	MCP4822_PORT |=  (1 << LDAC);

	_delay_us(100);	
}

void MCP4822_SHUTDOWN(void)
{
	SPI_SELECT();

	SPI_WRITE_BYTE(0x0);
	SPI_WRITE_BYTE(0x0);

	MCP4822_PORT &= ~(1 << LDAC);
	MCP4822_PORT |=  (1 << LDAC);

	SPI_DESELECT();
}

