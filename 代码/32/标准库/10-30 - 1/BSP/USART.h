#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

#define USART_REC_LEN 200
extern uint8_t USART_RX_BUF[USART_REC_LEN];
extern uint16_t USART_RX_STA;

extern void Usart_Gpio_Init(void);
extern void Usart_Nvic_Init(void);
extern void Usart_Init(uint32_t Baud);

#endif

