#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f10x.h"
#include "GPIO.h"

void EXTI_init(void);
void NVIC_init(void);


 extern volatile u8 flag;
#endif

