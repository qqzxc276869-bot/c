#include "stm32f10x.h"
#include "uart.h"

int main(void)
{
    /* 系统时钟 startup 文件已经调 SystemInit() 配成 72MHz，
       APB2 总线时钟 = 72MHz，所以第一个参数填 72 */
    Reg_Usart_Init(72, 115200);

    while (!(USART1->SR & (1u << 6)));  // 等 bit6=TC，一帧数据真正移位发完

    while (1)
    {
        /* 主循环里什么都不用写：
           - 接收：收到一个字节硬件置 RXNE，触发 USART1_IRQHandler，
             在中断里读 DR 又写回 DR，自动回显；
           - 发送：要发数据直接往 USART1->DR 写，
             写之前等 SR 的 bit7=TXE(发送寄存器空) 就行。        */
    }
}
