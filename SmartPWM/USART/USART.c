#include "USART.h"

FILE*usart;

void UASRT_INIT()
{
	UART_DDR  |=
				(
				(1<<UART_TX_PIN)|
				(1<<UART_EN_PIN)|
				(1<<PRT_RST_PIN)|
				(1<<PWR_CTR_PIN)
				);

	UART_PORT |=
				(
				(1<<UART_TX_PIN)|
				(1<<UART_EN_PIN)|
				(1<<PWR_CTR_PIN)
				);

	UART_PORT &=~(1<<PRT_RST_PIN);

	UART_DDR &=~(1<<UART_RX_PIN);

	UBRRH=0x00;
	UBRRL=0x22;									//USART 28.8K bod

	UCSRC=
	(
		 (0<<USBS) |
		 (1<<UCSZ0)|
		 (1<<UCSZ1)|
		 (1<<URSEL)
	);

	usart=fdevopen(USART_PUTCHAR,USART_GETCHAR);
	RS485_READ();
	USART_START();
}

int USART_PUTCHAR(char data)
{
	while ( !( UCSRA & (1<<UDRE) )) ;
	UDR=data;
	return 0;
}

int USART_GETCHAR()
{
	while ( !(UCSRA & (1<<RXC)));
	return	UDR;
}

void USART_START()
{
	UCSRB=
	(
		(1<<TXEN) |
		(1<<RXEN) |
		(0<<TXCIE)|
		(0<<RXCIE)
	);
}

void USART_STOP()
{
	UCSRB&=
	~(
		(1<<TXEN)|
		(1<<RXEN)|
		(0<<RXCIE)
	);
}

void RS485_READ(void)
{
	_delay_ms(10);
	UART_PORT &= ~(1 << UART_EN_PIN);	
}

void RS485_WRITE(void)
{
	UART_PORT |=  (1 << UART_EN_PIN);
	_delay_ms(10);
}