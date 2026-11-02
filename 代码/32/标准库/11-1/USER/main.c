#include "main.h"
uint32_t i;
int main()
{
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
delay_init();
pwm_init();

	while(1)
		{
			for (i = 0; i <= 100;i++)
			{
				pwm_setcompare2(i);
				delay_ms(10);
			}
				for (i = 0; i <= 100;i++)
			{
				pwm_setcompare2(100-i);
				delay_ms(10);
			}
			
		}
}
