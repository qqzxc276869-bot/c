#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

/*
 * 中断写入接收数据，主循环读取并回显。
 * 第15位表示接收完成，第14位表示已收到0D，低14位记录正文长度。
 */
extern volatile uint8_t USART_RX_BUF[200];
extern volatile uint16_t USART_RX_STA;

/* Plck：实际APB2时钟，单位MHz；bound：串口波特率。 */
void Reg_Usart_Init(uint16_t Plck, uint32_t bound);

#endif
