#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

typedef enum
{
    Short_Pressed = 1,
    Long_Pressed,
    Key_IDLE
} KEY_Sta;

typedef struct
{
    KEY_Sta Key_Sta;
    uint32_t Press_Count;
} KEY_T;

uint8_t Key_Short_Long_Pressed(KEY_T *Key_Stat,
                               GPIO_TypeDef *GPIOx,
                               uint16_t GPIO_Pin,
                               uint8_t Pressed_Level);

#endif
