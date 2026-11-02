/*
 * 本文件包含四部分：
 * 1. uart_control()：把命令编号转换为 GPIO 操作。
 * 2. uart_receive()：逐字节检查消息格式，完整正确后执行命令。
 * 3. uart_init() 和 USART1_IRQHandler()：初始化串口，并接收实际到达的字节。
 * 4. fputc()：供前台 printf 调试输出使用，不参与协议接收。
 *
 * 串口硬件只负责传送字节，并不知道 01 表示开灯。
 * “报头、命令、报尾和结束标志”的含义，由本文件中的解析程序实现。
 */
#include "uart.h"

/*
 * 以下是本例的 LED 编号约定：
 * LED0 接 PB5，LED1 接 PE5；两个 LED 都是低电平亮、高电平灭。
 * 蜂鸣器使能端接 PB8，高电平工作、低电平关闭。
 *
 * PORT 指定 GPIO 端口，PIN 指定端口中的引脚位掩码。
 * 例如 GPIO_Pin_5 表示选中第 5 号引脚，并不是把数值 5 写到引脚上。
 * 这些引脚的输出模式由 BSP/GPIO.c 的 Gpio_Init() 负责初始化。
 */
#define LED0_PORT  GPIOB
#define LED0_PIN   GPIO_Pin_5

#define LED1_PORT  GPIOE
#define LED1_PIN   GPIO_Pin_5

#define BEEP_PORT  GPIOB
#define BEEP_PIN   GPIO_Pin_8

/*
 * uart_state 表示“下一次收到的字节应该放在协议的哪个位置”。
 * 协议共有 8 个位置，下标从 0 开始：
 *
 * 下标：     0    1    2    3    4    5    6    7
 * 内容：    18   AA   BB   XX   CC   DD   0D   0A
 *
 * 例如 uart_state=2，表示前面的 18 AA 已匹配，接下来应该收到 BB。
 * 这些变量在多次中断之间保留状态，不会每进一次中断就重新清零。
 * 文件作用域的 static 还限制它们只能在当前 uart.c 中直接访问。
 */
static uint8_t uart_state = 0;

/*
 * 暂存第 4 个字节中的命令，例如 01 表示打开 LED1。
 * 接收到命令时只是保存；后续 CC DD 0D 0A 也正确后，才真正执行。
 * 即使一条错误消息留下旧命令，下一条完整消息也会先覆盖它再执行。
 */
static uint8_t uart_cmd = 0;


/*
 * 功能：根据收到的命令，控制一个 LED 或蜂鸣器。
 * 参数 cmd：协议中的第 4 个字节，由作业约定为 01～06。
 *
 * 从第一个 if 开始，依次判断 cmd 等于哪个命令。
 * 只要某个条件成立，就执行它的大括号内的语句，跳过后面的 else if。
 * 因此一条命令只会对应一种操作，不会把所有设备都操作一遍。
 *
 * GPIO_ResetBits() 将指定引脚输出置为低电平；
 * GPIO_SetBits() 将指定引脚输出置为高电平。
 * LED 是低电平亮，蜂鸣器是高电平响，需要按各自的有效电平控制。
 */
static void uart_control(uint8_t cmd)
{
    if (cmd == 0x01)
    {
        /*
         * 命令 01：打开 LED1。
         * LED1 接在 PE5，低电平点亮，所以把对应引脚输出清零。
         * 重复收到打开命令时仍保持亮，不使用“翻转”操作。
         */
        GPIO_ResetBits(LED1_PORT, LED1_PIN);
    }
    else if (cmd == 0x02)
    {
        /*
         * 前面的 01 不匹配时，再判断是不是 02。
         * 命令 02：关闭 LED1，将 PE5 输出置为高电平。
         */
        GPIO_SetBits(LED1_PORT, LED1_PIN);
    }
    else if (cmd == 0x03)
    {
        /*
         * 命令 03：打开另一个 LED，也就是 PB5 上的 LED0。
         * GPIO_ResetBits() 只清零选中的输出位，不影响其他引脚。
         */
        GPIO_ResetBits(LED0_PORT, LED0_PIN);
    }
    else if (cmd == 0x04)
    {
        /*
         * 命令 04：关闭 LED0，将 PB5 输出置高。
         * 本例 LED 低电平亮，所以引脚置高后，LED 反而熄灭。
         */
        GPIO_SetBits(LED0_PORT, LED0_PIN);
    }
    else if (cmd == 0x05)
    {
        /*
         * 命令 05：打开蜂鸣器，将 PB8 输出置高。
         * 本例按有源蜂鸣器或带驱动模块编写，高电平使其持续鸣叫。
         * 如果硬件需要方波驱动，应由定时器产生相应波形。
         */
        GPIO_SetBits(BEEP_PORT, BEEP_PIN);
    }
    else if (cmd == 0x06)
    {
        /*
         * 命令 06：关闭蜂鸣器，将 PB8 输出置低。
         * 此后保持关闭状态，直到收到下一条有效的打开命令。
         */
        GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
    }
    else
    {
        /*
         * 如果传入的值不属于 01～06，就不会操作任何输出。
         * 正常情况下，协议解析阶段已经排除了这样的命令。
         */
    }
}


/*
 * 功能：每次接收一个字节，按顺序检查完整指令。
 * 参数 data：本次中断实际收到的一个字节，不是一整条消息。
 *
 * 完整指令的 8 个字节依次为：
 * 第 1 个 18，第 2 个 AA，第 3 个 BB，第 4 个命令，
 * 第 5 个 CC，第 6 个 DD，第 7 个 0D，第 8 个 0A。
 *
 * 外层 if / else if 通过 uart_state 判断“现在等第几个字节”；
 * 内层 if / else 判断“这次收到的 data 是不是该位置需要的内容”。
 * 每次调用只进入一个外层分支，处理完就返回，等下一字节到达后再接着处理。
 *
 * 这里必须用 else if 串起各接收阶段：
 * 如果改成多个彼此独立的 if，uart_state 在前面改变后，同一个 data
 * 就可能又进入后面的阶段，造成一个字节被当成多个字节使用。
 *
 * 任一阶段出错时，当前 data 如果是 18，就把它当成新报头的开始；
 * 否则回到 uart_state=0，重新等待 18。本例不设置帧内超时。
 */
static void uart_receive(uint8_t data)
{
    if (uart_state == 0)
    {
        /*
         * 第一步：还没有开始接收一条有效消息，等待第 1 个字节 18。
         * 例如刚上电或上一条消息处理完后，都会从这里开始。
         */
        if (data == 0x18)
        {
            /* 第 1 个字节正确，下次收到数据时，进入等待 AA 的阶段。 */
            uart_state = 1;
        }
        else
        {
            /* 收到其他内容就忽略，继续停在第一步等待报头。 */
            uart_state = 0;
        }
    }
    else if (uart_state == 1)
    {
        /*
         * 第二步：前面已经收到 18，现在应该收到第 2 个字节 AA。
         * 只有当前 data 等于 AA，才允许进入下一步。
         */
        if (data == 0xAA)
        {
            uart_state = 2;
        }
        else
        {
            /*
             * 第 2 个字节错误，原来的消息作废。
             * 如果当前又收到 18，它可以作为新消息的第 1 个字节，
             * 所以继续保持下一步等待 AA；否则重新等待报头。
             */
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
    else if (uart_state == 2)
    {
        /*
         * 第三步：前面的 18 AA 已正确收到，现在检查第 3 个字节 BB。
         * BB 匹配成功后，下一次就可以接收实际控制命令。
         */
        if (data == 0xBB)
        {
            uart_state = 3;
        }
        else
        {
            /*
             * 本次不是 BB，放弃前面的残帧。
             * 当前字节若为 18，就利用它开始匹配下一条消息。
             */
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
    else if (uart_state == 3)
    {
        /*
         * 第四步：报头 18 AA BB 已完整收到，这个字节是实际命令。
         * 下面列出的六个数来自作业要求，对应两盏 LED 和蜂鸣器的开关。
         * || 表示“或者”：只要与其中任意一个命令相等，就符合要求。
         */
        if (data == 0x01 ||
            data == 0x02 ||
            data == 0x03 ||
            data == 0x04 ||
            data == 0x05 ||
            data == 0x06)
        {
            /*
             * 先把有效命令保存到 uart_cmd，再进入等待 CC 的阶段。
             * 此时只收到了前 4 个字节，还不能执行命令；
             * 要等后面的 CC DD 0D 0A 也全部正确。
             */
            uart_cmd = data;
            uart_state = 4;
        }
        else
        {
            /*
             * 如果是 00、07 等未规定的值，这条指令就无效。
             * 如果这个无效值恰好是 18，就把它当作新报头的开始。
             */
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
    else if (uart_state == 4)
    {
        /*
         * 第五步：报头和命令已收到，现在检查报尾的第一个字节 CC。
         * 当前 data 必须等于 CC，才能继续等待 DD。
         */
        if (data == 0xCC)
        {
            uart_state = 5;
        }
        else
        {
            /*
             * 报尾开头不正确，前面保存的命令不能执行。
             * 按当前字节是否为 18，决定是否已经开始收到新的报头。
             */
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
    else if (uart_state == 5)
    {
        /*
         * 第六步：前面的 CC 已收到，现在检查报尾的第二个字节 DD。
         * DD 正确后，才开始检查接收结束标志 0D 0A。
         */
        if (data == 0xDD)
        {
            uart_state = 6;
        }
        else
        {
            /*
             * 这次不是 DD，当前消息不能执行。
             * 新到的 18 可以作为下一条消息的开始，其他值直接丢弃。
             */
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
    else if (uart_state == 6)
    {
        /*
         * 第七步：报头、命令、报尾都正确，等待结束标志的第一个字节 0D。
         * 0D 是一个完整的数据字节，它与串口硬件的“停止位”含义不同。
         */
        if (data == 0x0D)
        {
            uart_state = 7;
        }
        else
        {
            /*
             * 结束标志的开头不正确，不能执行已经保存的命令。
             * 如果收到的是 18，则直接转去等待新报头的 AA。
             */
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
    else if (uart_state == 7)
    {
        /*
         * 第八步：前面 7 个字节已经正确，最后必须收到 0A。
         * 只有这个条件也成立，才代表一条完整、有效的指令已经收齐。
         */
        if (data == 0x0A)
        {
            /*
             * 先恢复到等待报头的阶段，为紧接着到来的下一条消息做准备。
             * 再执行第 4 个字节中保存的命令，对 LED 或蜂鸣器进行操作。
             */
            uart_state = 0;
            uart_control(uart_cmd);
        }
        else
        {
            /*
             * 即使前面 7 个字节正确，最后一个字节错误也不能执行。
             * 这里同样保留新报头 18，使后续的正确消息可以重新匹配。
             */
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
    else
    {
        /*
         * 按正常执行顺序，uart_state 始终只会是 0～7，不会进入这里。
         * 保留这个分支，在状态意外异常时重新寻找报头，恢复接收进度。
         */
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


/*
 * 功能：初始化 USART1，使其能够通过 PA9/PA10 收发数据并产生接收中断。
 * 参数 baud：波特率；本例由 main() 传入 9600。
 *
 * 接收中断生效需要两个层次都开启：
 * 1. USART_ITConfig() 允许串口外设在接收或错误事件发生时提出中断请求；
 * 2. NVIC_Init() 允许 CPU 响应该外设的中断请求，并设置其优先级。
 */
void uart_init(uint32_t baud)
{
    /*
     * 这三个结构体分别保存 GPIO、串口和中断控制器的配置参数。
     * 先填写结构体成员，再把结构体地址交给对应的标准库初始化函数。
     */
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /*
     * GPIOA 与 USART1 都挂在 APB2 总线上，使用前必须使能各自时钟。
     * | 是按位或，把两个外设时钟的选择位组合起来；
     * ENABLE 表示开启这些时钟，不是在这里设置串口波特率。
     */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
        ENABLE
    );

    /*
     * 将 USART1 恢复到复位配置，避免沿用此前可能存在的外设设置。
     * 接着清零软件接收状态，使第一条消息从等待报头开始。
     * 本函数安排在上电初始化阶段调用，此时接收中断尚未开启。
     */
    USART_DeInit(USART1);

    uart_state = 0;
    uart_cmd = 0;

    /*
     * 为 GPIO 配置结构体填入库提供的默认值。
     * 注意这里只修改内存中的结构体，还没有改变引脚硬件；
     * 后面的 GPIO_Init() 才会把配置写入外设。
     */
    GPIO_StructInit(&GPIO_InitStruct);

    /*
     * USART1 默认发送引脚是 PA9。
     * AF_PP 表示复用推挽输出，让串口外设控制发送引脚的高低电平。
     * 50MHz 是 GPIO 的输出速度等级，不是让引脚自动输出 50MHz 波形，
     * 也不是串口波特率；实际串口通信速度由 baud 参数决定。
     */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*
     * USART1 默认接收引脚是 PA10，使用 IPU，即内部上拉输入。
     * 上拉使未被外部设备拉低的输入倾向于保持高电平，符合 UART 空闲状态。
     * 这里复用同一个结构体，修改引脚和模式，再配置一次 GPIOA。
     * 输入模式不使用上面 GPIO_Speed 字段所表示的输出速度等级。
     */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*
     * 先为串口配置结构体填默认值，再逐项明确本例要使用的参数。
     * USART_Init() 会根据 USART1 所用的 PCLK2 时钟计算波特率配置。
     */
    USART_StructInit(&USART_InitStruct);

    /* 使用传入的波特率；电脑端必须选择相同数值。 */
    USART_InitStruct.USART_BaudRate = baud;

    /* 每个串口数据字包含 8 位数据，协议中的一个字节由此传输。 */
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;

    /* 每个串口数据字结束后跟 1 位停止位，与协议的 0D 0A 是不同层次。 */
    USART_InitStruct.USART_StopBits = USART_StopBits_1;

    /* 不使用奇偶校验位，对应串口助手中的“无校验”。 */
    USART_InitStruct.USART_Parity = USART_Parity_No;

    /* 不使用 RTS/CTS 硬件流控，只需收发线和公共地。 */
    USART_InitStruct.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;

    /* | 组合两个模式，允许串口同时具备接收和发送能力。 */
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* 将结构体中的设置写入 USART1，后面还需要单独使能串口。 */
    USART_Init(USART1, &USART_InitStruct);

    /*
     * RXNE 表示接收数据寄存器非空：有一个收到的数据字等待程序读取。
     * 使能 RXNE 中断后，正常收到字节就可以请求 CPU 进入接收中断。
     *
     * ERR 使能帧错误、噪声和溢出相关的错误中断，不包含独立的 PE 中断使能。
     * 接收函数会检查错误，并放弃受影响的残帧。
     */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_ERR, ENABLE);

    /* 外设参数设置完成后，使能 USART1，让它开始工作。 */
    USART_Cmd(USART1, ENABLE);

    /*
     * USART1_IRQn 指定 USART1 的中断通道。
     * 在 main() 已设置的分组 2 下，抢占优先级与子优先级都取 1。
     * NVIC_IRQChannelCmd=ENABLE 才允许 CPU 响应此通道的中断请求。
     */
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStruct);
}


/*
 * USART1 的中断入口，函数名称必须与启动文件中断向量表中的名称一致。
 * 它由硬件中断机制调用，不需要在 main() 的 while 循环中手动调用。
 * 工程中只能有一份同名函数定义，不能同时编译另一份串口接收实现。
 */
void USART1_IRQHandler(void)
{
    /* 存放本次从接收数据寄存器取出的一个字节。 */
    uint8_t data;

    /*
     * 先处理接收错误，再处理正常数据：
     * ORE：溢出，旧数据还没读走，新数据已经到达，说明可能丢了字节。
     * FE ：帧错误，例如收到的数据字没有被正确识别的停止位。
     * NE ：噪声错误，硬件检测到接收采样异常。
     * PE ：奇偶校验错误；本例未启用校验，正常配置下通常不会产生。
     *
     * || 表示只要任意一个错误标志成立，就进入错误处理。
     * 不把错误检查藏在 RXNE 分支内，是为了也处理仅有错误标志的情况。
     */
    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET ||
        USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
    {
        /*
         * STM32F1 的这些错误按“先读状态寄存器 SR，再读数据寄存器 DR”清除。
         * 上面的 USART_GetFlagStatus() 已经读了 SR，这里再读 DR 完成清除。
         * (void) 表示有意丢弃返回的数据，但函数的读取动作仍会实际执行。
         *
         * 出错的数据不交给解析器；把接收位置清零，等待下一条完整消息。
         */
        (void)USART_ReceiveData(USART1);
        uart_state = 0;
        return;
    }

    /*
     * GetITStatus() 会检查 RXNE 状态与对应中断使能，
     * != RESET 表示确实有待处理的接收数据。
     */
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
