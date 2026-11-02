#include "BUZZER.h"
#include "stm32f10x.h"
void BUZZER_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOA的时钟
                                                          // 使用各个外设前必须开启时钟，否则对外设的操作无效

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure; // 定义结构体变量

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // GPIO模式，赋值为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;        // GPIO引脚，赋值为所有引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度，赋值为50MHz

    GPIO_Init(GPIOB, &GPIO_InitStructure); // 将赋值后的构体变量传递给GPIO_Init函数
                                           // 函数内部会自动根据结构体的参数配置相应寄存器
                                           // 实现GPIOA的初始化
    GPIO_SetBits(GPIOB, GPIO_Pin_12);      // 设置GPIOA的第0号引脚为高电平，点亮BUZZER
}
void BUZZER_ON(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // 设置GPIOA的第1号引脚为低电平，点亮LED
}
void BUZZER_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    }
}
void BUZZER_OFF(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12); // 设置GPIOA的第1号引脚为高电平，关闭LED
}
