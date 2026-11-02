
#include "uart.h"
#define  Led0 GPIOB
#define  Led0_pin5 GPIO_Pin_5

#define  Led1 GPIOE
#define  Led1_pin5 GPIO_Pin_5

#define  Beep GPIOB
#define  Beep_pin8 GPIO_Pin_8

//  * 下标：     0    1    2    3    4    5    6    7
//  * 内容：    18   AA   BB   XX   CC   DD   0D   0A

static u8 uart_state=0;
static u8 uart_cmd=0;

static void uart_control(u8 cmd)
{
    if(cmd==0x01)
    {
        GPIO_ResetBits(Led0, Led0_pin5); // 打开led0;
    }
    else if(cmd==0x02)
    {
        GPIO_SetBits(Led0, Led0_pin5);//关闭led0
    }


    if(cmd==0x03)
    {
        GPIO_ResetBits(Led1, Led0_pin5); // 打开led1;
    }
    else if(cmd==0x04)
    {
        GPIO_SetBits(Led1, Led0_pin5);//关闭led1
    }
      else if(cmd==0x05)
    {  
        GPIO_SetBits(Beep, Beep_pin8);//打开beep
    }
    else if(cmd==0x06)
    {
         GPIO_ResetBits(Beep, Beep_pin8);//关闭beep
    }
    else
    {

    }
}
//  * 下标：     0    1    2    3    4    5    6    7
//  * 内容：    18   AA   BB   XX   CC   DD   0D   0A
static void uart_receive(uint8_t data)
{
if(uart_state==0)
    {
        if(data==0x18)
        {
            uart_state = 1;
        }
        else
        {
            uart_state = 0;
        }
    }
else if(uart_state==1)
    {
         if(data==0xAA)
        {
            uart_state = 2;
        }
        else
        {
            if(data==0x18)
              {
              uart_state = 1;
              }
            else
             {
               uart_state = 0;
             }
        }
    }
else if(uart_state==2)
    {
         if(data==0xBB)
        {
            uart_state = 3;
        }
        else
        {
            if(data==0x18)
              {
              uart_state = 1;
              }
            else
             {
               uart_state = 0;
             }
        }
    }
else if(uart_state==3)
     {
         if(data==0x01||data==0x02||data==0x03||data==0x04||data==0x05||data==0x06)
        {
            uart_cmd = data;
            uart_state = 4;
        }
        else
        {
            if(data==0x18)
              {
              uart_state = 1;
              }
            else
             {
               uart_state = 0;
             }
        }
    }

else if(uart_state==4)
    {
         if(data==0xCC)
        {
            uart_state = 5;
        }
        else
        {
            if(data==0x18)
              {
              uart_state = 1;
              }
            else
             {
               uart_state = 0;
             }
        }
    }
else if(uart_state==5)
    {
         if(data==0xDD)
        {
            uart_state = 6;
        }
        else
        {
            if(data==0x18)
              {
              uart_state = 1;
              }
            else
             {
               uart_state = 0;
             }
        }
    }
else if(uart_state==6)
    {
         if(data==0x0D)
        {
            uart_state = 7;
        }
        else
        {
            if(data==0x18)
              {
              uart_state = 1;
              }
            else
             {
               uart_state = 0;
             }
        }
    }
else if(uart_state==7)
    {
         if(data==0x0A)
        {
            uart_state = 0;
            uart_control(uart_cmd);
        }
        else
        {
            if(data==0x18)
              {
              uart_state = 1;
              }
            else
             {
               uart_state = 0;
             }
        }
    }
else
{
         if (data == 0x18)
        {
            uart_state = 1;
        }
        else
        {
            uart_state = 0;
        }
}
}
void uart_init(uint32_t baud)
{    GPIO_InitTypeDef GPIO_InitStructure;
     USART_InitTypeDef USART_InitStruct;
       NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_USART1, ENABLE);

    /* 定义 GPIO 初始化结构体 */

    
    /* 配置 GPIO 工作模式 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 推挽输出模式
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9 ;                    // 选择 GPIO 引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // GPIO 输出速度 50MHz
    
    /* 初始化 GPIOA */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* 配置 GPIO 工作模式 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;      // 推挽输出模式
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10 ;                    // 选择 GPIO 引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // GPIO 输出速度 50MHz
    
    /* 初始化 GPIOA */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //uart
   USART_DeInit(USART1);

    uart_state = 0;
    uart_cmd = 0;
   
    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength= USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ERR , ENABLE);
    USART_Cmd(USART1, ENABLE);

 
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStruct);
}
void USART1_IRQHandler(void)
{
    u8 data = 0;
      if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
    {
        /*
         * 上面已经读取过 SR，这里再读取 DR，
         * 就完成了“先读 SR，再读 DR”的错误清除过程。
         *
         * 这个数据可能有错误，所以直接丢弃。
         * (void) 表示不使用返回值，读取动作仍然会执行。
         */
        (void)USART_ReceiveData(USART1);

        /*
         * 接收出错后，前面收到的残帧不能继续使用。
         * 将软件接收状态恢复为 0，重新等待报头 18。
         */
        uart_state = 0;

        /*
         * 结束本次中断处理。
         * 不让有错误的数据继续进入下面的正常解析流程。
         */
        return;
    }
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        /*
         * USART_ReceiveData() 读取接收数据寄存器。
         * 本例使用 8 位数据，转换为 uint8_t 后交给解析器。
         * 读取 DR 会清除正常的 RXNE 标志，无需再手动清除接收挂起位。
         */
        data = (uint8_t)USART_ReceiveData(USART1);

        /*
         * 每次只传入本次收到的一个字节，解析器使用 uart_state 记住进度。
         * 例如先后收到 18、AA、BB 时，会分别执行三次中断和三次本函数调用；
         * 只有最后的 0A 也验证成功，才会调用 uart_control()。
         */
        uart_receive(data);
    }
}

/*
 * 提供逐字符发送的 fputc 实现，供工程现有的 printf 重定向配置使用。
 * 例如前台调用 printf("OK") 时，需要依次发送字符 O 和 K。
 * 本协议不要求发送回执，所以接收中断没有调用这个函数。
 */
int fputc(int ch, FILE *f)
{
    /*
     * 标准 fputc 接口包含文件参数，但本实现固定发送到 USART1，不区分文件。
     * 转为 void 表示有意不使用 f，同时避免未使用参数的编译警告。
     */
    (void)f;

    /*
     * TXE=1 表示发送数据寄存器空闲，可以再写入下一个数据字。
     * 若仍为 RESET，就在这里等待，因此这是阻塞式发送函数。
     *
     * TXE 不等于整个字符已经从引脚发送完毕；
     * 若需要确认最后一个字符在线路上发送完成，应检查 TC 标志。
     */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

    /*
     * 把 ch 的低 8 位交给串口发送，硬件会继续完成移位输出。
     * 返回 ch 表示本函数已接收这个待发送字符，不保证物理发送此刻已结束。
     */
    USART_SendData(USART1, (uint8_t)ch);

    return ch;
}
