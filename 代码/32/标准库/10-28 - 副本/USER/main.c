#include "main.h"

int main(void)
{
    u16 len, i;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(9600);

    while (1)
    {
        /* 确认一批数据接收完成 */
        if (USART_RX_STA & 0x8000)
        {
            /* 取出本批数据的长度 */
            len = USART_RX_STA & 0x3FFF;

            /* 逐字节发送回去 */
            for (i = 0; i < len; i++)
            {
                USART_SendData(USART1, USART_RX_BUF[i]);

                /* 等待这个字节发送完成 */
                while (!(USART1->SR & 0x40));
            }

            /* 处理完毕，允许接收下一批 */
            USART_RX_STA = 0;
        }
    }
}
