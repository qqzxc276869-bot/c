#include "stm32f10x.h"
#include "GPIO.h"
#include "key.h"
#include "stm32f10x_it.h"  /* 使用工程中已有的 Tick_ms 声明 */

static KEY_T key_pa0 = {Key_IDLE, 0};
static KEY_T key_pe4 = {Key_IDLE, 0};
static KEY_T key_pe3 = {Key_IDLE, 0};
static KEY_T key_pe2 = {Key_IDLE, 0};

int main(void)
{
    uint32_t last_tick;
    uint32_t current_tick;
    uint8_t event;

    SystemCoreClockUpdate();

    /* 现有 SysTick_Handler 每 1 ms 给 Tick_ms 加 1 */
    if (SysTick_Config(SystemCoreClock / 1000) != 0)
    {
        while (1)
        {
        }
    }

    Gpio_Init();
    Key_Init();
    last_tick = Tick_ms;
    while (1)
    {
        current_tick = Tick_ms;

        if (current_tick == last_tick)
        {
            continue;
        }

        last_tick = current_tick;

        /* PA0：按下为高电平 */
        event = Key_Short_Long_Pressed(&key_pa0,
                                   GPIOA, GPIO_Pin_0, Bit_SET);
        if (event == 1)  /* 短按 */
        {
            Led_Turn(GPIOB, GPIO_Pin_5);
        }
        else if (event == 2)  /* 长按 */
        {
            Led_Turn(GPIOE, GPIO_Pin_5);
        }

        /* PE4：按下为低电平 */
        event = Key_Short_Long_Pressed(&key_pe4,
                                   GPIOE, GPIO_Pin_4, Bit_RESET);
        if (event == 1)  /* 短按 */
        {
            Led_Turn(GPIOB, GPIO_Pin_5);
        }
        else if (event == 2)  /* 长按 */
        {
            Led_Turn(GPIOE, GPIO_Pin_5);
        }

        /* PE3：按下为低电平 */
        event = Key_Short_Long_Pressed(&key_pe3,
                                   GPIOE, GPIO_Pin_3, Bit_RESET);
        if (event == 1)  /* 短按 */
        {
            Led_Turn(GPIOB, GPIO_Pin_5);
        }
        else if (event == 2)  /* 长按 */
        {
            Led_Turn(GPIOE, GPIO_Pin_5);
        }

        /* PE2：按下为低电平 */
        event = Key_Short_Long_Pressed(&key_pe2,
                                   GPIOE, GPIO_Pin_2, Bit_RESET);
        if (event == 1)  /* 短按 */
        {
            Led_Turn(GPIOB, GPIO_Pin_5);
        }
        else if (event == 2)  /* 长按 */
        {
            Led_Turn(GPIOE, GPIO_Pin_5);
        }
    }
}
