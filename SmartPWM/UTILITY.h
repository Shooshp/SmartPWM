#ifndef UTILITY_H_
#define UTILITY_H_

#define SIZE_RECEIVE_BUF  128
#define POWER_SOURCE_PREFIX 0xA0

#include <avr/io.h>
#include "USART/USART.h"
#include "MCP4822/MCP4822.h"

void SET_CURRENT(uint16_t VALUE);
void SET_VOLTAGE(uint16_t VALUE);
void POWER_ON (void);
void POWER_OFF (void);
uint8_t GET_ADDRESS (void);

#endif /* UTILITY_H_ */