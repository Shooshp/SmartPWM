#include "SPI.h"
#include <avr/io.h>
#include <avr/interrupt.h>
    
void SPI_INIT()
// Initialize pins for spi communication
{
	DDR_SPI &= ~((1<<DD_MOSI)|(1<<DD_MISO)|(1<<DD_SS)|(1<<DD_SCK));
	// Define the following pins as output

	DDR_SPI |= ((1<<DD_MOSI)|(1<<DD_SS)|(1<<DD_SCK));
	  
	SPCR =  
	(
  	 (1<<SPE)| // SPI Enable
	(0<<SPIE)| // SPI Interrupt Enable
	(0<<DORD)| // Data Order (0:MSB first / 1:LSB first)
	(1<<MSTR)| // Master/Slave select
	(1<<SPR1)| // SPI Clock Rate
	(1<<SPR0)| // SPI Clock Rate
	(0<<CPOL)| // Clock Polarity (0:SCK low / 1:SCK hi when idle)
	(0<<CPHA)  // Clock Phase (0:leading / 1:trailing edge sampling)
	);
	  
	SPSR = (1<<SPI2X);              // Double Clock Rate

	PORT_SPI |= ( 1 << CS_PIN );
}
  
void SPI_WRITE_READ (uint8_t * dataout, uint8_t * datain, uint8_t len)
// Shift full array through target device
{
	for (uint8_t i = 0; i < len; i++) 
	{
		SPDR = dataout[i];
		while((SPSR & (1<<SPIF))==0);
		datain[i] = SPDR;
	}
}
  
void SPI_WRITE (uint8_t * dataout, uint8_t len)
// Shift full array to target device without receiving any byte
{
	for (uint8_t i = 0; i < len; i++)
	{
		SPDR = dataout[i];
		while((SPSR & (1<<SPIF))==0);
	}
}

void SPI_WRITE_BYTE (uint8_t  dataout)
{
	SPDR = dataout;
	while((SPSR & (1<<SPIF))==0);
}
  
uint8_t SPI_FAST_SHIFT (uint8_t data)
// Clocks only one byte to target device and returns the received one
{
	SPDR = data;
	while((SPSR & (1<<SPIF))==0);
	return SPDR;
}

void SPI_SELECT(void)
{
	PORT_SPI &= ~( 1 << CS_PIN );
}

void SPI_DESELECT(void)
{
	PORT_SPI |= ( 1 << CS_PIN );
}