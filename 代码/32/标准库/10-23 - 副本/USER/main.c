
#include "main.h"
int main()
{
	uint8_t stat = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	Gpio_Init();
	KEY_Init();
	EXTI_init();
	NVIC_init();
	while(1)
		{
//			stat = Key_Scanf(0);
//			switch (stat)
//			{
//				case 1:Led_Turn(GPIOB,GPIO_Pin_5);break;
//			}
		}
	
	
}
