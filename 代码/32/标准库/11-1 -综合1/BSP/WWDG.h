#ifndef __WWDG_H
#define __WWDG_H
#include "stm32f10x.h"
void WWDG_NVIC_INIT(void);
void Wwdg_Init(uint32_t WWDG_Prescaler,uint8_t WindowValue,uint8_t Counter);
#endif