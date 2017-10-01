#ifndef MCP4822_H_
#define MCP4822_H_

#include <avr/io.h>
#include <util/delay.h>
#include "../SPI/SPI.h"

#define MCP4822_PORT  PORTC
#define MCP4822_DDR   DDRC
#define LDAC          PINC6

void MCP4822_INIT (void);
void MCP4822_SET(uint8_t CHANEL, uint16_t DATA);
void MCP4822_SHUTDOWN(void);


#endif /* MCP4822_H_ */