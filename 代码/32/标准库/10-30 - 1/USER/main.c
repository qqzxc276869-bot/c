#include "main.h"

int main()
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	gpio_Init();
	tim_init(4999, 7199);

	while(1)
		{ 
			
		}
}
