#ifndef PARSER_H
#define PARSER_H

#include <avr/io.h>

/*вместимость приемного буфера*/
#define SIZE_RECEIVE_BUF  128

#define ADDRESS_PORT  PORTC
#define ADDRESS_DDR   DDRC

#define CRC_CONSTANT  0xA001

extern  char RECIVE_FLAG;

/*функции парсера*/
void PARSER_INIT(void);
uint8_t GET_ADDRESS(void);
uint16_t CHECK_CRC16 (uint8_t DATA_SIZE);


#endif //PARSER_H