#include "stm32f10x.h"
#include "gpio.h"
#include "IWDG.h"
/* 放在 main() 前面；本函数使用 SysTick 实现毫秒延时。 */
void Delay_ms(uint32_t ms)
{
    /* 先停止计数，再设置每1毫秒需要的时钟次数。 */
    SysTick->CTRL = 0;
    SysTick->LOAD = SystemCoreClock / 1000 - 1;
    SysTick->VAL = 0;

    /*
     * 使用CPU时钟，开启计数，不开启中断。
     * 0x05：位2和位0为1，位1为0。
     */
    SysTick->CTRL = 0x05;

    while (ms > 0)
    {
        /* 位16变为1，表示完成了一次计数，即经过约1毫秒。 */
        while ((SysTick->CTRL & (1U << 16)) == 0)
        {
        }

        ms--;
    }

    /* 延时结束，停止计数。 */
    SysTick->CTRL = 0;
}
int main(void)
{
  gpio_init();
    GPIOB->ODR &= ~(1u << 5);
  GPIOE->ODR &= ~(1u << 5);
   GPIOB->ODR |= (1u << 8);

  /* 延时 500 ms */
  Delay_ms(500);
  GPIOB->ODR |= (1u << 5);//B5,高电平
  GPIOE->ODR |= (1u << 5);//E5,高电平
   GPIOB->ODR &= ~(1u << 8); // B8,蜂鸣器,低电平6

  wwdg_init();
   while (1)
   {
     if ((GPIOA->IDR & (1u << 0)) != 0u)
     {
       IWDG->KR = 0xAAAA;
     }

  }
}
