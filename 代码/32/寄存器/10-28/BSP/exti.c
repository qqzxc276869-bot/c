#include "exti.h"
#include "stm32f10x.h"
void exti_init()
{
    RCC->APB2ENR = 0x1u; // 打开复用时钟;
    //中断映射
    AFIO->EXTICR[0] = 0x00u;
    //中断参数配置
    EXTI->RTSR = 0x01;//上升
    EXTI->FTSR &= ~0x01u;
    //配置中断分组
    NVIC->ISER[0] |= 0x01 << 6;
    NVIC->IP[1] |= 0x00 << 16;

}
void EXTI0_IRQnHandler()
{
    if(EXTI->PR&0x01u)
    {
        EXTI->PR = 0x00001u;
    }
}