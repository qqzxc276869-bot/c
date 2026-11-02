#include "uart.h"

volatile uint8_t UART_RxData = 0;
volatile uint8_t UART_RxFlag = 0;

/**
 * 串口1初始化
 * baud：波特率，例如9600、115200
 */
void uart_init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 1. 开启GPIOA和USART1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
    /* 2. 复位USART1 */
    USART_DeInit(USART1);

    /* 3. 配置PA9：USART1_TX，复用推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 4. 配置PA10：USART1_RX，浮空输入 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 5. 配置串口参数 */
    USART_StructInit(&USART_InitStructure);

    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);

    /* 6. 配置USART1中断通道 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

    /* 优先级分组2：这两个优先级的取值范围都是0～3 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 7. 开启接收中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* 8. 使能USART1 */
    USART_Cmd(USART1, ENABLE);
}

/* 发送一个字节 */
void UART_SendByte(uint8_t data)
{
    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

    USART_SendData(USART1, data);
}

/* USART1中断服务函数：收到一个字节后自动进入 */
void USART1_IRQHandler(void)
{
    u8 data = 0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        /* 读取接收数据，同时清除RXNE标志 */
        data = (uint8_t)USART_ReceiveData(USART1);

       USART_SendData(USART1,data);
    }
}