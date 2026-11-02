#include "PWM.h"

void pwm_init(void)
{
    GPIO_InitTypeDef GPIO_InitTypeStruct;
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
        TIM_OCInitTypeDef TIM_OCInitTypeStruct;
	//2、使能相应外设的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 
	//3、往结构体中设置相应参数
    GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8;//LED1  BBEP
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//4、调用初始化函数进行初始化
	GPIO_Init(GPIOB, &GPIO_InitTypeStruct);
	// GPIO_ResetBits(GPIOB,GPIO_Pin_5);



RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 99;//APR
    TIM_TimeBaseInitStruct.TIM_Prescaler = 719;//PSC
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);


    TIM_OCStructInit(&TIM_OCInitTypeStruct);
    // TIM_OCInitTypeStruct.TIM_OCIdleState=
    TIM_OCInitTypeStruct.TIM_OCMode = TIM_OCMode_PWM1 ;
    // TIM_OCInitTypeStruct.TIM_OCNIdleState=
    // TIM_OCInitTypeStruct.TIM_OCNPolarity=
    TIM_OCInitTypeStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    // TIM_OCInitTypeStruct.TIM_OutputNState=
    TIM_OCInitTypeStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitTypeStruct.TIM_Pulse = 90;//CCR
    TIM_OC2Init(TIM3, &TIM_OCInitTypeStruct);

    TIM_Cmd(TIM3,ENABLE);

}

void pwm_setcompare2(u16 compare)
{
    TIM_SetCompare2(TIM3, compare);
}