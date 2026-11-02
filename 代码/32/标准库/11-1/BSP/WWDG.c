#include "WWDG.h"
#include "GPIO.h"
void WWDG_NVIC_INIT(void)
{
	NVIC_InitTypeDef NVIC_InitTypeStruct;
	NVIC_InitTypeStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitTypeStruct);
}
void Wwdg_Init(uint32_t WWDG_Prescaler,uint8_t WindowValue,uint8_t Counter)
{
	//打开看门狗时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler);
	WWDG_SetWindowValue(WindowValue);
	WWDG_Enable(Counter);
	WWDG_ClearFlag();
	WWDG_NVIC_INIT();
	WWDG_EnableIT();
}


void WWDG_IRQHandler(void)
{
	if(WWDG_GetFlagStatus())
	{
		WWDG_SetCounter(0x7f);
		
		Led_Turn(GPIOE,GPIO_Pin_5);
		WWDG_ClearFlag();
	}
	
	
}


