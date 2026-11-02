
#include "stm32f10x.h"

void Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitTypeStruct);

}
void delay_us(u16 time)
{
	while(time--)
	{
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();

	}

}
void delay_ms(u16 time)
{
	while(time--)
	{
		delay_us(1000);		
	}
}
int main()
{
	Gpio_Init();
		while(1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		delay_ms(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		delay_ms(500);
	}
	
	
}
