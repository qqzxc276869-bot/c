#include "EXTI.h"

void EXTI_init(void)
{
	EXTI_InitTypeDef EXTI_InitTypeStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//当使用外部中断映射时，需要打开复用时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	EXTI_InitTypeStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitTypeStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitTypeStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitTypeStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitTypeStruct);
}

void NVIC_init(void)
{
	NVIC_InitTypeDef NVIC_InitTypeStruct;
	NVIC_InitTypeStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitTypeStruct);
}

//1、无参无返回值的函数
//2、只需要定义，不需要声明和调用
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		Led_Turn(GPIOB,GPIO_Pin_5);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	
}



