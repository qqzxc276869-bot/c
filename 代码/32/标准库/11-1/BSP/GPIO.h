#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f10x.h"
#include "sys.h"

#define KEY_UP GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY_0  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY_1  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define KEY_2  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)

#define  LED0  PBout(5)

void gpio_Init(void);
void KEY_Init(void);
//void delay_us(u16 time);
//void delay_ms(u16 time);
uint8_t Key_Scanf(uint8_t mode);
void Led_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Beep_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#endif


