#include "USART.h"

FILE*usart;

void UASRT_INIT()
{	
	UART_DDR  |=
				(
				(1<<UART_TX_PIN)|
				(1<<UART_EN_PIN)|
				(1<<PRT_RST_PIN)| //Защита от перенапруги
				(1<<PWR_CTR_PIN)  //Включение ДЦ ДЦ
				);

	UART_PORT |=
				(
				(1<<UART_TX_PIN)|
				(1<<UART_EN_PIN)|
				(1<<PWR_CTR_PIN) //Включение ДЦ ДЦ
				);

	UART_PORT &=~(1<<PRT_RST_PIN);

	UART_DDR &=~(1<<UART_RX_PIN);

	UBRRH=0x00;
	UBRRL=0x33;		//Для внутренних 8 МГц часов это 9600

	UCSRC=
	(
		 (0<<USBS) | // Один стоповый бит
		 (1<<UCSZ0)| // Приём по 8 бит
		 (1<<UCSZ1)| // Приём по 8 бит
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
		(1<<TXEN) |   // - Writing this bit to one enables the USART Transmitter.
		(1<<RXEN) |   // - Writing this bit to one enables the USART Receiver.
		(1<<RXCIE)	  // - Writing this bit to one enables interrupt on the RXC Flag. A USART Receive Complete Interrupt
					  // will be generated only if the RXCIE bit is written to one, the Global Interrupt Flag in SREG is written
					  // to one and the RXC bit in UCSRA is set.
	);
}

void USART_STOP()
{
	UCSRB&=
	~(
		(1<<TXEN)|
		(1<<RXEN)|
		(1<<RXCIE)
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