#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <util/delay.h>
#include "../MCP4822/MCP4822.h"

#define ADC_PORT PORTA
#define ADC_DDR  DDRA
#define VRef 2.481

void ADC_INIT (void);
uint16_t ADC_GET_VALUE(uint8_t CHANEL);
int GET_VOLTAGE(void);
int GET_CURRENT(void);
int ADC_CALIBRATION(void);



#endif /* ADC_H_ */