#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

extern volatile uint8_t UART_RxData;  // 接收到的数据
extern volatile uint8_t UART_RxFlag;  // 收到数据的标志

void uart_init(uint32_t baud);
void UART_SendByte(uint8_t data);

#endif