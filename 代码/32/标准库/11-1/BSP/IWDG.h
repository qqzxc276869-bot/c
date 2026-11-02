#ifndef __IWDG_H
#define __IWDG_H
#include "stm32f10x.h"

void Iwdg_Init(uint8_t IWDG_Prescaler,uint16_t Reload);
void Eat_Dog(void);
#endif

