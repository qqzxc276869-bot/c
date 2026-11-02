#ifndef _GPIO_H
#define _GPIO_H
#include "stm32f10x.h"
void light_init(void);
void light_on(void);
void light_off(void);
void light_turn(GPIO_TypeDef *GPIOX, uint16_t GPIO_PIN);

#endif
