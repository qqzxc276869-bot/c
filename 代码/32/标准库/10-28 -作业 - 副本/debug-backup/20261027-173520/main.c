#include "stm32f10x.h"
#include "uart.h"

int main(void)
{
    uint8_t data;

    /* 设置中断优先级分组，整个工程通常设置一次即可 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* 初始化USART1，波特率9600 */
    uart_init(9600);

    while (1)
    {

    }
}