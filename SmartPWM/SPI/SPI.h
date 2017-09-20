#ifndef _SPI_H_
#define _SPI_H_
#include <avr/io.h>

#define PORT_SPI    PORTB
#define DDR_SPI     DDRB

#define DD_MISO     DDB6
#define DD_MOSI     DDB5
#define DD_SS       DDB4
#define DD_SCK      DDB7

#define CS_PIN      PINB4
 
void SPI_INIT();
void SPI_WRITE_READ (uint8_t * dataout, uint8_t * datain, uint8_t len);
void SPI_WRITE (uint8_t * dataout, uint8_t len);
void SPI_WRITE_BYTE (uint8_t dataout);

void SPI_SELECT();
void SPI_DESELECT();

uint8_t SPI_FAST_SHIFT (uint8_t data);

 
#endif /* _SPI_H_ */