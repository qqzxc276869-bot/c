#include "key.h"
#include "stm32f10x_gpio.h"

/*
 * 每 1 ms 调用一次。
 * Pressed_Level：该按键按下时的电平，Bit_SET 或 Bit_RESET。
 * 短按下限 50 ms，长按阈值 800 ms。
 * 返回值：0 无事件，1 短按，2 长按。
 */
uint8_t Key_Short_Long_Pressed(KEY_T *Key_Stat,
                               GPIO_TypeDef *GPIOx,
                               uint16_t GPIO_Pin,
                               uint8_t Pressed_Level)
{
    uint8_t Pin_Level;
    uint8_t Pressed;
    uint8_t Event;

    /* 真正从引脚读取电平 */
    Pin_Level = GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);

    if (Pin_Level == Pressed_Level)
    {
        Pressed = 1;
    }
    else
    {
        Pressed = 0;
    }

    switch (Key_Stat->Key_Sta)
    {
    case Key_IDLE:
        if (Pressed == 1)
        {
            Key_Stat->Press_Count = 1;
            Key_Stat->Key_Sta = Short_Pressed;
        }
        break;

    case Short_Pressed:
        if (Pressed == 1)
        {
            Key_Stat->Press_Count++;

            if (Key_Stat->Press_Count >= 800)
            {
                Key_Stat->Key_Sta = Long_Pressed;
                return 2;  /* 长按 */
            }
        }
        else
        {
            if (Key_Stat->Press_Count >= 50)
            {
                Event = 1;  /* 短按 */
            }
            else
            {
                Event = 0;
            }

            Key_Stat->Press_Count = 0;
            Key_Stat->Key_Sta = Key_IDLE;
            return Event;
        }
        break;

    case Long_Pressed:
        /* 长按事件已经触发，只等待松手 */
        if (Pressed == 0)
        {
            Key_Stat->Press_Count = 0;
            Key_Stat->Key_Sta = Key_IDLE;
        }
        break;

    default:
        Key_Stat->Press_Count = 0;
        Key_Stat->Key_Sta = Key_IDLE;
        break;
    }

    return 0;
}
