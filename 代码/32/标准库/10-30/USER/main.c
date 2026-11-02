#include "main.h"

int main()
{
	uint16_t len,i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	Gpio_Init();
	KEY_Init();
	EXTI_init();
	NVIC_init();
	Usart_Gpio_Init();

	Usart_Init(115200);

	Usart_Nvic_Init();
	Beep_Turn(GPIOB, GPIO_Pin_8);
	delay_ms(500);
	Wwdg_Init(WWDG_Prescaler_8,0X5F,0X7F);

	
	while(1)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		}
}
