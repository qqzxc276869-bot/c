#include "TIM.h"

void tim_init(u32 TIM_Period,u32  TIM_Prescaler)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = TIM_Period;
    TIM_TimeBaseInitStruct.TIM_Prescaler = TIM_Prescaler;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStruct);
    TIM_Cmd(TIM2, ENABLE);//开始计数
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        /*
         * 清除本次更新中断标志。
         * 否则退出后标志仍然存在，会反复进入中断。
         */
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        /* 读取当前输出电平，再设置为相反电平，实现LED翻转。 */
        if (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5) == Bit_SET)
        {
            GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        }
        else
        {
            GPIO_SetBits(GPIOE, GPIO_Pin_5);
        }
    }
}