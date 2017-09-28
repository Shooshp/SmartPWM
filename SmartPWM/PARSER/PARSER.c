#include "PARSER.h"

#define TRUE   1
#define FALSE  0


void PARSER_INIT(void)
{
	ADDRESS_DDR  &= ~((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3));
}

#ifdef  __GNUC__

uint8_t PARS_EqualStrFl(char *s1, char const *s2)
{
	uint8_t i = 0;
	
	while(s1[i] == pgm_read_byte(&s2[i]) && s1[i] != '\0' && pgm_read_byte(&s2[i]) != '\0')
	{
		i++;
	}
	
	if (s1[i] =='\0' && pgm_read_byte(&s2[i]) == '\0')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#else

uint8_t PARS_EqualStrFl(char *s1, char __flash *s2)
{
	uint8_t i = 0;
	
	while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	
	if (s1[i] =='\0' && s2[i] == '\0')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#endif


uint8_t PARS_EqualStr(char *s1, char *s2)
{
	uint8_t i = 0;
	
	while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	
	if (s1[i] =='\0' && s2[i] == '\0')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

uint8_t PARS_StrToUchar(char *s)
{
	uint8_t value = 0;
	
	while(*s == '0')
	{
		s++;
	}
	
	while(*s)
	{
		value += (*s - 0x30);
		s++;
		if (*s)
		{
			value *= 10;
		}
	};
	
	return value;
}

uint16_t PARS_StrToUint(char *s)
{
	uint16_t value = 0;
	
	while(*s == '0')
	{
		s++;
	}
	
	while(*s)
	{
		value += (*s - 0x30);
		s++;
		if (*s)
		{
			value *= 10;
		}
	};
	
	return value;
}