#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <util/delay.h>

#define UART_PORT	PORTD
#define UART_DDR	DDRD

#define UART_RX_PIN	 PIND0
#define UART_TX_PIN	 PIND1
#define UART_EN_PIN	 PIND3
#define PRT_RST_PIN  PIND6 // сброс защиты от перенапряжения 
#define PWR_CTR_PIN	 PIND7 // отключение LM2596 по низкому уровню на этом выводе

#define ADDRESS_PORT  PORTB
#define ADDRESS_DDR   DDRB
#define ADDRESS_PIN   PINB

void UASRT_INIT(void);
char USART_PUTCHAR(char data);
char USART_GETCHAR();
void USART_START(void);
void USART_STOP(void);
void RS485_READ(void);
void RS485_WRITE(void);

#endif /* USART_H_ */