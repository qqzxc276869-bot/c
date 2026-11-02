#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "kEY.h"
uint8_t key_num;
int main(void)
{
    LED_Init(); // 初始化LED
    key_init(); // 初始化按键
    while (1)
    {
        key_num = key_Getnum(); // 获取按键值
        if (key_num == 1)
        {
            LED1_Turn();
        }
        if (key_num == 2)
        {
            LED2_Turn();
        }
    }
}
