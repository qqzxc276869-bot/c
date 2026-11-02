#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "BUZZER.h"
#include "Choose.h"

int main(void)
{
    BUZZER_Init();
    Choose_Init();
    while (1)
    {
        if (Choose_Get() == 1)

        {
            BUZZER_ON();
        }
        else
        {
            BUZZER_OFF();
        }
    }
}
