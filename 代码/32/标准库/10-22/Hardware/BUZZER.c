#include "BUZZER.h"
#include "stm32f10x.h"
void BUZZER_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启 GPIOB 时钟
                                                          // 使用各个外设前必须开启时钟，否则对外设的操作无效

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure; // 定义结构体变量

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // GPIO模式，赋值为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;        // 蜂鸣器控制引脚 PB8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度，赋值为50MHz

    GPIO_Init(GPIOB, &GPIO_InitStructure); // 将赋值后的构体变量传递给GPIO_Init函数
                                           // 函数内部会自动根据结构体的参数配置相应寄存器
                                           // 实现 GPIOB 的初始化
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);      // PB8 = 0，S8050 截止，蜂鸣器默认关闭
}
void BUZZER_ON(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_8); // PB8 = 1，S8050 导通，蜂鸣器通电
}
void BUZZER_Turn(GPIO_TypeDef *GPIOX,uint16_t GPIO_PIN)
{
  GPIO_WriteBit(GPIOX,GPIO_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOX, GPIO_PIN)));
}

void BUZZER_OFF(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_8); // PB8 = 0，S8050 截止，蜂鸣器断电
}
