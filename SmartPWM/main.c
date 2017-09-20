#define	F_CPU (16000000UL)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "SPI/SPI.h"
#include "MCP4822/MCP4822.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
#include "PARSER/PARSER.h"

#define OFFSET 5

PROGMEM const char cmd_0[]="I";
PROGMEM const char cmd_1[]="U";
PROGMEM const char cmd_2[]="On";
PROGMEM const char cmd_3[]="Off";
PROGMEM const char cmd_4[]="Stat";



int main(void)
{
	uint16_t VALUE_0 = 0;
	uint16_t VALUE_1 = 0;

	MCP4822_INIT();
	ADC_INIT();
	UASRT_INIT();
	PARS_Init();

	MCP4822_SET(0,0);
	MCP4822_SET(1,0);
	stdin=usart;

	DDRC  |= (1<<PINC7);
	PORTC &=~(1<<PINC7);

	INT0_DDR &=~(1<<INT0_PIN);
	GICR = 1<<INT0;

	MCUCR  &=
	~(
		(1 << ISC01)|
		(1 << ISC00)
	);

//	sei();

	RS485_WRITE();
	fprintf(usart, "SMART PWM IS READY\r\nSOFTWARE VERSION: 1.0\r\n");
	RS485_READ();
	
    while (1) 
    {
		PARS_Parser(getc(usart));
    }
}

void PARS_Handler(uint8_t argc, char *argv[])
{
	if (PARS_EqualStrFl(argv[0],cmd_0)==1)
	{
		int tmp=PARS_StrToUint(argv[1]);
				
		SET_CURRENT(tmp);

		RS485_WRITE();
		fprintf(usart, "\r\nCurrent limit set to: %umA\r\n", tmp);	
		RS485_READ();	
	}

	if (PARS_EqualStrFl(argv[0],cmd_1)==1)
	{
		int tmp=PARS_StrToUint(argv[1]);
		int tmp2 = tmp;
		char FLAG = 1;
				
		SET_VOLTAGE(tmp);
		_delay_ms(100);

		int VOLTAGE;
		/*
		while(FLAG)
		{
			VOLTAGE = GET_VOLTAGE();

			if (VOLTAGE > (tmp + OFFSET))
			{
				tmp2--;
				SET_VOLTAGE(tmp2);
				_delay_ms(10);
			}
			else
			{
				if (VOLTAGE < (tmp - OFFSET))
				{
					tmp2++;
					SET_VOLTAGE(tmp2);
					_delay_ms(10);
				} 
				else
				{
					FLAG = 0;
				}
			}
		}
		*/
		RS485_WRITE();
		fprintf(usart, "\r\nVoltage limit set to: %umV\r\n", tmp);	
		RS485_READ();	
	}

	if (PARS_EqualStrFl(argv[0],cmd_2)==1)
	{				
		POWER_ON();

		RS485_WRITE();
		fprintf(usart, "\r\nOutput is online!\r\n");	
		RS485_READ();
	}

	if (PARS_EqualStrFl(argv[0],cmd_3)==1)
	{				
		POWER_OFF();

		RS485_WRITE();
		fprintf(usart, "\r\nOutput is offline!\r\n");	
		RS485_READ();
	}

	if (PARS_EqualStrFl(argv[0],cmd_4)==1)
	{				
		int tmp1 = GET_VOLTAGE();
		int tmp2 = GET_CURRENT();
		RS485_WRITE();
		fprintf(usart, "\r\nVoltage: %umV\r\nCurrent: %umV\r\n", tmp1,tmp2);	
		RS485_READ();
	}
}

void SET_CURRENT(int VALUE)
{
	if(VALUE > 3000)
	{
		VALUE = 3000;
	}

	VALUE = VALUE * 1.363;

	MCP4822_SET(1, VALUE);
}

void SET_VOLTAGE(int VALUE)
{
	int DATA = VALUE; 

	if (DATA > 20000)
	{
		DATA = 20000;
	}

	DATA = DATA / 4.97;


	MCP4822_SET(0, DATA);
}

void POWER_ON (void)
{
	UART_PORT &= ~(1 << PWR_CTR_PIN);
	_delay_ms(10);
	UART_PORT |=  (1 << PRT_RST_PIN);
	_delay_ms(100);
	UART_PORT &= ~(1 << PRT_RST_PIN);
};

void POWER_OFF (void)
{
	UART_PORT |=  (1 << PWR_CTR_PIN);
	_delay_ms(10);
};

