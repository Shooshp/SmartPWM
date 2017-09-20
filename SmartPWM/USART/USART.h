#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define UART_PORT	PORTD
#define UART_DDR	DDRD

#define UART_RX_PIN	 PIND0
#define UART_TX_PIN	 PIND1
#define UART_EN_PIN	 PIND3
#define PRT_RST_PIN  PIND6
#define PWR_CTR_PIN	 PIND7


FILE*usart;

void UASRT_INIT(void);
int  USART_PUTCHAR(char data);
int  USART_GETCHAR();
void USART_START(void);
void USART_STOP(void);
void RS485_READ(void);
void RS485_WRITE(void);

#endif /* USART_H_ */