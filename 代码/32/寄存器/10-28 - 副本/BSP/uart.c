#include "uart.h"

/* 中断和主循环共享这些变量，volatile要求实际访问其内容。 */
volatile uint8_t USART_RX_BUF[200];
volatile uint16_t USART_RX_STA = 0;

/* Plck：实际APB2时钟，单位MHz；bound：串口波特率。 */
void Reg_Usart_Init(uint16_t Plck, uint32_t bound)
{
    /* 避免无效参数导致后面的波特率计算除以0。 */
    if (Plck == 0 || bound == 0)
    {
        return;
    }

    /* 开启GPIOA和USART1时钟，读回一次，确保时钟使能生效。 */
    RCC->APB2ENR |= (1u << 2) | (1u << 14);
    (void)RCC->APB2ENR;

    /*
     * PA9：50MHz复用推挽输出，CRH对应的4位应写0xB。
     * 0x8表示上下拉输入，不能用作串口发送引脚的输出模式。
     */
    GPIOA->CRH &= ~(0xFu << 4);
    GPIOA->CRH |=  (0xBu << 4);

    /* PA10：浮空输入，接收外部设备发送的数据。 */
    GPIOA->CRH &= ~(0xFu << 8);
    GPIOA->CRH |=  (0x4u << 8);

    /*
     * 配置期间先关闭串口。
     * CR1/CR2/CR3清零，对应8位数据、无校验、1位停止位、无硬件流控。
     */
    USART1->CR1 = 0;
    USART1->CR2 = 0;
    USART1->CR3 = 0;

    /*
     * 使用传入的APB2时钟计算BRR，不再写死为72MHz。
     * Plck乘1000000换算成Hz；加bound/2，使除法结果接近四舍五入。
     */
    USART1->BRR = ((uint32_t)Plck * 1000000u + bound / 2u) / bound;

    /* 先读SR，再读DR，清除可能残留的接收、空闲及接收错误标志。 */
    (void)USART1->SR;
    (void)USART1->DR;
    USART_RX_STA = 0;

    /* 优先级分组2：2位抢占优先级、2位子优先级。 */
    SCB->AIRCR = (0x05FAu << 16) | (5u << 8);
    NVIC->IP[USART1_IRQn] = (3u << 6) | (3u << 4);

    /* USART1中断号37，使用第2组寄存器的第5位。 */
    NVIC->ICPR[1] = (1u << 5);
    NVIC->ISER[1] = (1u << 5);

    /*
     * 开启USART1、RXNE接收中断、发送和接收。
     * 本例用0D 0A判断消息结束，不开启IDLE空闲中断。
     */
    USART1->CR1 = (1u << 13) |
                  (1u << 5)  |
                  (1u << 3)  |
                  (1u << 2);
}

/* 收到一个字节后进入，按0D 0A结尾接收一条消息。 */
void USART1_IRQHandler(void)
{
    u8 data;
    uint32_t status;

    /*
     * 读取SR保存当前状态。
     * SR低4位分别为PE、FE、NE、ORE，任意一位置1都表示接收错误。
     */
    status = USART1->SR;
    if ((status & 0x0Fu) != 0u)
    {
        /* 前面已读SR，这里读DR，清除错误并丢弃有问题的数据。 */
        (void)USART1->DR;

        /* 残帧作废；已完成、等待主循环回显的消息仍保留。 */
        if ((USART_RX_STA & 0x8000) == 0)
        {
            USART_RX_STA = 0;
        }
        return;
    }

    /*
     * &运算必须先用括号括起来，再与0比较。
     * 同时检查RXNE标志和RXNEIE，确认确有接收数据需要处理。
     */
    if (((status & (1u << 5)) != 0u) &&
        ((USART1->CR1 & (1u << 5)) != 0u))
    {
        /* 读取DR后，正常的RXNE标志会由硬件自动清除。 */
        data = (u8)(USART1->DR & 0xFFu);

        /*
         * 一条消息完成后，先保持缓冲区内容不变，等待主循环回显。
         * 主循环回显完会清零USART_RX_STA，再开始接收下一条。
         */
        if ((USART_RX_STA & 0x8000) == 0)
        {
            if (USART_RX_STA & 0x4000)
            {
                /* 已经收到0D，接下来必须是0A。 */
                if (data == 0x0A)
                {
                    USART_RX_STA |= 0x8000;
                }
                else
                {
                    /* 0D后面不是0A，放弃这条残帧，重新开始。 */
                    USART_RX_STA = 0;
                }
            }
            else
            {
                if (data == 0x0D)
                {
                    /*
                     * 只置位“已收到0D”的标志。
                     * 使用|=，保留低14位中已接收的正文长度。
                     */
                    USART_RX_STA |= 0x4000;
                }
                else
                {
                    /* 存入当前字节，再将接收计数增加1。 */
                    USART_RX_BUF[USART_RX_STA & 0x3FFF] = data;
                    USART_RX_STA++;

                    /*
                     * 保留原来的长度限制：正文最多199字节。
                     * 收到第200个正文数据字节时，放弃本条并重新计数。
                     */
                    if (USART_RX_STA > 199)
                    {
                        USART_RX_STA = 0;
                    }
                }
            }
        }
    }
}
