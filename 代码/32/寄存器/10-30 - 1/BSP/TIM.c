#include "TIM.h"
#include "stm32f10x.h"

void tim_init()
{
    RCC->APB1ENR = 0x1u << 0;
    TIM2->SMCR &=~ (0x07 << 0);
    TIM2->CR1 &= ~((0x03 << 8) | (0x01 << 4)); //复位
    TIM2->CR1 |= (0x00 << 8) | (0x00 << 4);//设置分频以及向上计数

    TIM2->PSC = 7199;
    TIM2->ARR = 4199;
    TIM2->EGR |= 1 << 0;//更新事件
    TIM2->SR &= ~(1 << 0);//清除事件标志
    TIM2->DIER |= 1 << 0;//中断使能

    NVIC->IP[TIM2_IRQn] = (1 << 4) | (1 << 0);
    NVIC->ISER[0] = (1 << TIM2_IRQn);
    
    TIM2->CR1 |= (1 << 0);
}

void TIM2_IRQHandler(void)
{
    /* 查询更新中断标志：SR 位0 = UIF（中断已使能，直接查标志即可） */
    if ((TIM2->SR & (1 << 0)) != 0)    // 等价 TIM_GetITStatus(TIM2, TIM_IT_Update)
    {
        /* 先清除 UIF，否则退出中断后会反复进入 */
        TIM2->SR &= ~(1 << 0);         // 等价 TIM_ClearITPendingBit(TIM2, TIM_IT_Update)

        /* 翻转 PE5 电平：读 ODR 判断当前电平，用 BSRR 一次性置位/清零 */
        if ((GPIOE->ODR & (1 << 5)) != 0)         // 当前输出高电平
        {
            GPIOE->BSRR = (1 << (5 + 16));        // BSRR 高16位是复位功能，PE5 清零
        }
        else
        {
            GPIOE->BSRR = (1 << 5);               // BSRR 低16位是置位功能，PE5 置1
        }
    }
}