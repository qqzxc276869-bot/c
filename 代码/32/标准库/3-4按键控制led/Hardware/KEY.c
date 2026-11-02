#include "stm32f10x.h"
#include "delay.h"
void key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB的时钟
                                                          // 使用各个外设前必须开启时钟，否则对外设的操作无效

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure; // 定义结构体变量

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // GPIO模式，赋值为上拉输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11; // GPIO引脚，赋值为第0号和第1号引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO速度，赋值为50MHz

    GPIO_Init(GPIOB, &GPIO_InitStructure); // 将赋值后的构体变量传递给GPIO_Init函数
                                           // 函数内部会自动根据结构体的参数配置相应寄存器
                                           // 实现GPIOB的初始化
}
uint8_t key_Getnum(void)
{
    uint8_t keynum = 0;
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        Delay_ms(10); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
            ;         // 等待按键释放
        Delay_ms(10); // 延时消抖
        keynum = 1;
    }
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
    {
        Delay_ms(10); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
            ;         // 等待按键释放
        Delay_ms(10); // 延` 时消抖
        keynum = 2;
    }
    return keynum;
}