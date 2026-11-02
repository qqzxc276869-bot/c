#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"
#include <stdio.h>  
void uart_init(uint32_t baud);
int fputc(int ch, FILE *f);
extern uint8_t USART_RX_BUF[200];
extern volatile uint16_t USART_RX_STA;
#endif
