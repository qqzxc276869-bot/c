#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
typedef enum
{
    zero =0,
   one ,
   two 
} key_;
typedef struct
{
    key_ key_state;
    u32 count;
} key;
uint8_t key_press(key *key, GPIO_TypeDef *gpiox, u16 GPIO_Pin, u16 press_state);

#endif
