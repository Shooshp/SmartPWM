#ifndef PARSER_H
#define PARSER_H

#include <avr/io.h>

#ifdef __GNUC__
#include <avr/pgmspace.h>
#endif

/*вместимость приемного буфера*/
#define SIZE_RECEIVE_BUF  128

#define ADDRESS_PORT  PORTB
#define ADDRESS_DDR   DDRB
#define ADDRESS_PIN   PINB

#define CRC_CONSTANT  0xA001

extern  char RECIVE_FLAG;

/*функции парсера*/
void PARSER_INIT(void);
uint8_t GET_ADDRESS(void);
uint16_t CHECK_CRC16 (uint8_t DATA_SIZE);

uint8_t  PARS_EqualStr(char *s1, char *s2);
uint8_t  PARS_StrToUchar(char *s);
uint16_t PARS_StrToUint(char *s);

#ifdef __GNUC__
uint8_t PARS_EqualStrFl(char *s1, char const *s2);
#else
uint8_t PARS_EqualStrFl(char *s1, char __flash *s2);
#endif


#endif //PARSER_H