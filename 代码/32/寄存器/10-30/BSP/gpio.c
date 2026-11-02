#include "gpio.h"
#include "stm32f10x.h"
void gpio_init()
{
    RCC->APB2ENR |= (1u << 2) | (1u << 3) | (1u << 6);//开启时钟
    GPIOA->ODR &= ~(1u << 0); // A0下拉模式,配置为低电平
    GPIOB->ODR |= (1u << 5);//B5,高电平
    GPIOB->ODR &= ~(1u << 8); // B8,蜂鸣器,低电平
    GPIOE->ODR |= (1u << 5);//E5,高电平
    GPIOE->ODR |= (1u << 4);//E4,下拉模式,高电平
    GPIOA->CRL = (GPIOA->CRL &= ~(0xfu << 0)) | (0x8u << 0);//A0配置下拉输模式
    GPIOB->CRL = (GPIOB->CRL &= ~(0xfu << 20)) | (0x2u << 20);//B5推挽输出
    GPIOE->CRL = (GPIOE->CRL &= ~(0xfu << 20)) | (0x2u << 20);//E5推挽输出
    GPIOE->CRL = (GPIOE->CRL &= ~(0xfu << 16)) | (0x8u << 16);//E4,上拉输入
    GPIOB->CRH = (GPIOB->CRH &= ~(0xfu << 0)) | (0x2u << 0);//B8,推挽输出
}
void key_getnum()
{
    u16 pa0 = ((GPIOA->IDR & (1u << 0)) != 0u); // A0被按下;
    u16 pe4 = ((GPIOE->IDR & (1u << 4)) == 0u); // E4被按下;
    if(pa0)
    {
        GPIOB->ODR &= ~(1u << 5);
    }
    else
    {
        GPIOB->ODR |= (1u << 5);//B5,高电平
    }
    if(pe4)
    {
        GPIOE->ODR &= ~(1u << 5);//E5,低电平
    }
    else
    {
        GPIOE->ODR |= (1u << 5);//E5,高电平
    }
    if(pa0&&pe4)
    {
        GPIOB->ODR |= (1u << 8);//蜂鸣器高电平
    }
    else
    {
        GPIOB->ODR &= ~(1u << 8);//蜂鸣器低电平
    }
}
