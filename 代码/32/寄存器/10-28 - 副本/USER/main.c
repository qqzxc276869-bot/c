#include "stm32f10x.h"
#include "uart.h"

int main(void)
{
    u16 len;
    u16 i;

    /* 当前工程的APB2时钟为72MHz，串口波特率为115200。 */
    Reg_Usart_Init(72, 115200);

    while (1)
    {
        /*
         * 收到完整的0D 0A后，第15位被中断置1。
         * 低14位保存正文长度，不包含末尾的0D和0A。
         */
        if (USART_RX_STA & 0x8000)
        {
            len = USART_RX_STA & 0x3FFF;

            /*
             * 按顺序回显正文。TXE为1表示发送数据寄存器可以写入。
             * 发送放在主循环中，接收中断只负责收数据和判断结束标志。
             */
            for (i = 0; i < len; i++)
            {
                while ((USART1->SR & (1u << 7)) == 0u)
                {
                }
                USART1->DR = USART_RX_BUF[i];
            }

            /* 再发回0D和0A，让回显也以回车换行结尾。 */
            while ((USART1->SR & (1u << 7)) == 0u)
            {
            }
            USART1->DR = 0x0D;

            while ((USART1->SR & (1u << 7)) == 0u)
            {
            }
            USART1->DR = 0x0A;

            /*
             * TC为1表示最后一个字节已在线路上发送完成。
             * 发送完成后清零接收状态，允许接收下一条消息。
             * 本例一次处理一条，收到上一条回显后再发送下一条。
             */
            while ((USART1->SR & (1u << 6)) == 0u)
            {
            }
            USART_RX_STA = 0;
        }
    }
}
