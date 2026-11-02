#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

void Gpio_Init(void);
void Key_Init(void);
void Led_Turn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif
