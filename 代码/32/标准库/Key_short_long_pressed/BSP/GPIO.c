#include "GPIO.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* PB5 红灯、PE5 绿灯、PB8 蜂鸣器 */
void Gpio_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOE, ENABLE);

    /* 先设初始电平：两个 LED 熄灭，蜂鸣器关闭 */
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/* PA0 下拉；PE2、PE3、PE4 上拉 */
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 |
                               GPIO_Pin_3 |
                               GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void Led_Turn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    if (GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin) == Bit_SET)
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
    }
    else
    {
        GPIO_SetBits(GPIOx, GPIO_Pin);
    }
}
