#include "PARSER.h"

#define TRUE   1
#define FALSE  0


void PARSER_INIT(void)
{
	ADDRESS_DDR  &= ~((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3));
	ADDRESS_PORT &= ~((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3));
}




