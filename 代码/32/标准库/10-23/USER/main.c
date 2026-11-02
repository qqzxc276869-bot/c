#include "main.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* SysTick 每 1 ms 产生一次中断 */
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 1000U) != 0U)
    {
        while (1)
        {
        }
    }

    Gpio_Init();
    KEY_Init();
    exti();
    nvic();

    while (1)
    {
        if (key_event == 1)
        {
            key_event = 0;
            Led_Turn(GPIOB, GPIO_Pin_5);  /* 短按翻转 PB5 */
        }
        else if (key_event == 2)
        {
            key_event = 0;
            Led_Turn(GPIOE, GPIO_Pin_5);  /* 长按翻转 PE5 */
        }
    }
}
