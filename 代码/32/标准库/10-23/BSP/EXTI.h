#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f10x.h"

/* 这里只声明，变量定义放在 EXTI.c */
extern volatile uint32_t Tick_ms;
extern volatile uint32_t key_event;

void exti(void);
void nvic(void);
void EXTI0_IRQHandler(void);

#endif
