#include "KEY.h"
#include "stm32f10x.h"
#include "Delay.h"
#include "GPIO.h"
#include "BUZZER.h"

void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 开启 GPIOA、GPIOE 时钟 */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE,
        ENABLE
    );

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* KEY_UP：PA0，下拉输入，按下为高电平 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* KEY0、KEY1、KEY2：PE4、PE3、PE2
       上拉输入，按下为低电平 */
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/* KEY_UP：PA0 */
void key_scan1(void)
{
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
    {
        Delay_ms(10);  // 按下消抖

        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
        {
            light_turn(GPIOB, GPIO_Pin_5);
            light_turn(GPIOE, GPIO_Pin_5);
            BUZZER_Turn(GPIOB, GPIO_Pin_8);

            /* 等待 KEY_UP 松开 */
            while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
            {
            }

            Delay_ms(10);  // 松开消抖
        }
    }
}

/* KEY0：PE4 */
void key_scan2(void)
{
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
    {
        Delay_ms(10);

        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
        {
            light_turn(GPIOB, GPIO_Pin_5);
            light_turn(GPIOE, GPIO_Pin_5);
            BUZZER_Turn(GPIOB, GPIO_Pin_8);

            /* 等待 KEY0 松开 */
            while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
            {
            }

            Delay_ms(10);
        }
    }
}

/* KEY1：PE3 */
void key_scan3(void)
{
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)
    {
        Delay_ms(10);

        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)
        {
            light_turn(GPIOB, GPIO_Pin_5);
            light_turn(GPIOE, GPIO_Pin_5);
            BUZZER_Turn(GPIOB, GPIO_Pin_8);

            /* 等待 KEY1 松开 */
            while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)
            {
            }

            Delay_ms(10);
        }
    }
}

/* KEY2：PE2 */
void key_scan4(void)
{
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)
    {
        Delay_ms(10);

        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)
        {
            light_turn(GPIOB, GPIO_Pin_5);
            light_turn(GPIOE, GPIO_Pin_5);
            BUZZER_Turn(GPIOB, GPIO_Pin_8);

            /* 等待 KEY2 松开 */
            while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)
            {
            }

            Delay_ms(10);
        }
    }
}