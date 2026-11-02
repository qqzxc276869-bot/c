#include "uart.h"

/* Plck: APB2 时钟(MHz)，bound: 波特率 */
void Reg_Usart_Init(uint16_t Plck, uint32_t bound)
{
    float temp = 0;
    uint16_t Mantissa = 0;
    uint16_t Fraction = 0;

    temp = (float)(Plck * 1000000) / (bound * 16);
    Mantissa = temp;
    Fraction = (temp - Mantissa) * 16;
    Mantissa <<= 4;        // BRR 高 12 位放整数部分
    Mantissa += Fraction;  // 低 4 位放小数部分
    RCC->APB2ENR |= (1u << 14) | (1u << 2);
    GPIOA->CRH &= ~(0xf< 4);
    GPIOA->CRH |= (0xbu) << 4;//pa9,复用推挽输出

    GPIOA->CRH &= ~(0xfu< 8);
    GPIOA->CRH |= 0x4u << 8;//pa10,浮空输入

    USART1->CR1 = (0x01 << 2) | (0x01 << 3) | (0x01 << 4) | (0x01 << 5) | (0x01 << 13);
    USART1->BRR=Mantissa;

  NVIC->ISER[1] |= 1<<5;
	NVIC->IP[9] |= 0X00;

}

/* USART1 中断服务函数 */
void USART1_IRQHandler(void)
{
    u8 data = 0;
    if(USART1->SR&0x20)
    {
        data = USART1->DR;
        USART1->DR = data;
    }
}
