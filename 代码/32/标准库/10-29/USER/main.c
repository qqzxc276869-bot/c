#include "GPIO.h"
#include "IWDG.h"
#include "delay.h"

int main(void)
{
    Gpio_Init();
    Delay_Init();

    /* 上电后，LED亮、蜂鸣器响。 */
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOB, GPIO_Pin_8);

    Delay_Ms(200);  /* 保持200毫秒。 */

    /* 关闭LED和蜂鸣器。 */
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);

    /* 启动看门狗，超时时间约1秒。 */
    IWDG_Init();

    while (1)
    {
        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)!=SET)
        {   
             IWDG_ReloadCounter();
        /* 故意不喂狗，约1秒后单片机自动重启。 */

        }
     
    }
}