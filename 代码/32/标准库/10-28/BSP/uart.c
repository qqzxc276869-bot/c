#include "uart.h"
  // 提供 FILE 和 printf、fputc 的声明

/**
 * 串口1初始化
 * baud：波特率，例如9600、115200
 */
void uart_init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
     GPIO_Init(GPIOA, &GPIO_InitStruct);



    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1,&USART_InitStruct);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruct);
}
void  USART1_IRQHandler  ()
{

    u8 data = 0;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
    {
        data = USART_ReceiveData(USART1);
          /* 等待发送数据寄存器空闲 */
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {
        }
        USART_SendData(USART1, data);
    }
}
int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}
