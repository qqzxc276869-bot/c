#include "EXTI.h"
//中断初始化配置
void EXTI_init(void)
{
	EXTI_InitTypeDef EXTI_InitTypeStruct;
	///PA0  KEY_UP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//1、当使用外部中断映射时，需要打开复用时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//2、中断的映射
	//3、中断结构体参数配置
	EXTI_InitTypeStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitTypeStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitTypeStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitTypeStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitTypeStruct);
	//   PE4 KEY_0   
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);//2、中断的映射
	EXTI_InitTypeStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitTypeStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitTypeStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitTypeStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitTypeStruct);
	
	//PE2  KEY_2 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);//2、中断的映射
	EXTI_InitTypeStruct.EXTI_Line = EXTI_Line2;
	EXTI_InitTypeStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitTypeStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitTypeStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitTypeStruct);
	//PE3  KEY_1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);//2、中断的映射
	EXTI_InitTypeStruct.EXTI_Line = EXTI_Line3;
	EXTI_InitTypeStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitTypeStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitTypeStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitTypeStruct);
}
//4、中断分组的配置
void NVIC_init(void)
{
	NVIC_InitTypeDef NVIC_InitTypeStruct;
	NVIC_InitTypeStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitTypeStruct);
	
	NVIC_InitTypeStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitTypeStruct);
	
	NVIC_InitTypeStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitTypeStruct);
	
	NVIC_InitTypeStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitTypeStruct);
	
	
}
//5、配置中断服务函数
 volatile u8 flag;
//1、无参无返回值的函数
//2、只需要定义，不需要声明和调用
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		Led_Turn(GPIOB,GPIO_Pin_5);
		flag = 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

