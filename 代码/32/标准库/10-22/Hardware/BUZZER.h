#ifndef _BUZZER_H
#define _BUZZER_H
#include "stm32f10x.h"
void BUZZER_Init(void);
void BUZZER_ON(void);
void BUZZER_Turn(GPIO_TypeDef *GPIOX, uint16_t GPIO_PIN);
void BUZZER_OFF(void);

#endif
