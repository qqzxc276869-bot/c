#include "stm32f10x.h"
#include "GPIO.h"
#include "key.h"
#include "stm32f10x_it.h"  /* 使用工程中已有的 Tick_ms 声明 */

static key pa0 = {0,0};
static key pe2 = {0,0};
static key pe3 = {0,0};
static key pe4 = {0,0};

int main(void)
{
      u8 event;
        u32 last_time = Tick_ms;
    SystemCoreClockUpdate();
    if(SysTick_Config(SystemCoreClock/1000u)!=0)
    {
        while (1)
        {
            /* code */
        }
        
    }
    Gpio_Init();
    Key_Init();
  
  
    while (1)
    {
        u32 current = Tick_ms;
        if (current == last_time)
        {
            continue;
            /* code */
        }
        last_time = current;
        event = key_press(&pa0, GPIOA, GPIO_Pin_0, Bit_SET);
        if (event == 1)
        {
            Led_Turn(GPIOB, GPIO_Pin_5);
        }
        else if (event == 2)
        {
            Led_Turn(GPIOE, GPIO_Pin_5);
            /* code */
        }


        event = key_press(&pe2, GPIOE, GPIO_Pin_2, Bit_RESET);
     if(event==1)
     {
         Led_Turn(GPIOB, GPIO_Pin_5);
     }
     else if (event==2)
     {
         Led_Turn(GPIOE, GPIO_Pin_5);
         /* code */
     }


        event = key_press(&pe3, GPIOE, GPIO_Pin_3, Bit_RESET);
     if(event==1)
     {
         Led_Turn(GPIOB, GPIO_Pin_5);
     }
     else if (event==2)
     {
         Led_Turn(GPIOE, GPIO_Pin_5);
         /* code */
     }


        event = key_press(&pe4, GPIOE, GPIO_Pin_4, Bit_RESET);
     if(event==1)
     {
         Led_Turn(GPIOB, GPIO_Pin_5);
     }
     else if (event==2)
     {
         Led_Turn(GPIOE, GPIO_Pin_5);
         /* code */
     }
   }
}
