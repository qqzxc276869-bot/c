#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "BUZZER.h"
#include"KEY.h"
#include "GPIO.h"


int main(void)
{
    key_init();
    BUZZER_Init();
    light_init(); 

   while (1)
{
    key_scan1();
    key_scan2();
    key_scan3();
    key_scan4();

}
}
