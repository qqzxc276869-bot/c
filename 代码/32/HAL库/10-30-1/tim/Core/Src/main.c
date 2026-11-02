#include "main.h"
#include "tim.h"
#include "gpio.h"

void SystemClock_Config(void);

int main(void)
{
    /* 初始化HAL库和系统滴答定时器。 */
    HAL_Init();

    /* 配置系统时钟，继续使用你原来的这个函数。 */
    SystemClock_Config();

    /* 初始化PE5，让它能够输出电平、控制LED。 */
    gpio_init();

    /*
     * 配置TIM2的分频、计数上限等参数。
     * 这个函数内部也会开启TIM2时钟并配置NVIC。
     */
    MX_TIM2_Init();

    /*
     * 初始化可能产生更新标志，先清除。
     * 避免刚启动就立即进入一次中断。
     */
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);

    /* 开启更新中断并启动计数，从这里开始定时。 */
    HAL_TIM_Base_Start_IT(&htim2);
    while (1)
    {
      HAL_Delay(500);
    }
}