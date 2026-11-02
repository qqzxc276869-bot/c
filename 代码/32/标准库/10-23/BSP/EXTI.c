#include "EXTI.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
volatile uint32_t Tick_ms=0;
volatile uint32_t key_event = 0;
static uint32_t press_time = 0;
static uint32_t pressflag = 0;
void exti(void)
{
    EXTI_InitTypeDef EXTI_InitTySturct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitTySturct.EXTI_Line = EXTI_Line0;
    EXTI_InitTySturct.EXTI_LineCmd = ENABLE;
    EXTI_InitTySturct.EXTI_Mode =  EXTI_Mode_Interrupt ;
    EXTI_InitTySturct.EXTI_Trigger =EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitTySturct);
    EXTI_ClearITPendingBit(EXTI_Line0);
}
void nvic(void)
{
    NVIC_InitTypeDef NVIC_InitTyStruct;
    NVIC_InitTyStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitTyStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitTyStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitTyStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    NVIC_Init(&NVIC_InitTyStruct);
}
void EXTI0_IRQHandler(void)
{
    uint32_t hold_time;
    if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
        {
            if(pressflag==0)
            {
                press_time = Tick_ms;
                pressflag = 1;
            }
            
        }
        else if (pressflag==1)
        {
            hold_time = Tick_ms - press_time;
            pressflag = 0;
            if(hold_time >= 1000U)
            {
                key_event = 2;
            }
            else if(hold_time >= 50U)
            {
                key_event = 1;
                /* code */
            }
            
            /* code */
        }
        
    }
}
