# STM32F103ZE：GPIO、EXTI、NVIC、中断、串口、看门狗、定时器步骤

**本文件为标准库版。其他版本：**[HAL 库版](C:/Users/27686/Desktop/嵌入式/笔记/32/STM32F103ZE_初始化步骤_HAL库版.md) · [寄存器版](C:/Users/27686/Desktop/嵌入式/笔记/32/STM32F103ZE_初始化步骤_寄存器版.md)。三种版本分别使用。

标准外设库；PA0 下拉输入，PB5 控制低电平点亮的 LED；USART1 使用 PA9/PA10，9600、8N1。

按键初始化顺序：**GPIO → EXTI → NVIC**。准备代码放在 `.c` 文件开头，中断和发送函数独立定义；已有同名定义时直接沿用。串口沿用第三部分的优先级分组。

## 准备：头文件和配置变量

```c
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"
#include "stm32f10x_tim.h"
#include "misc.h"

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
USART_InitTypeDef USART_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

volatile uint8_t Key_Event = 0;
volatile uint8_t USART1_RxData = 0;  /* 保存最近接收的一个字节 */
volatile uint8_t WWDG_EarlyEvent = 0;
volatile uint8_t TIM3_Event = 0;
```

## 一、GPIO 初始化

### 1. 开启 GPIOA、GPIOB 时钟

```c
RCC_APB2PeriphClockCmd(
    RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,
    ENABLE
);
```

### 2. 设置 PB5 初始电平，LED 熄灭

```c
GPIO_SetBits(GPIOB, GPIO_Pin_5);
```

### 3. 配置 PB5 推挽输出

```c
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_Init(GPIOB, &GPIO_InitStructure);
```

### 4. 配置 PA0 下拉输入

```c
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
```

## 二、EXTI 初始化

### 1. 开启 AFIO 时钟

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
```

### 2. 将 PA0 映射到 EXTI0

```c
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
```

### 3. 配置 EXTI0 上升沿中断

```c
EXTI_InitStructure.EXTI_Line = EXTI_Line0;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);
```

### 4. 清除 EXTI0 旧挂起

```c
EXTI_ClearITPendingBit(EXTI_Line0);
```

## 三、NVIC 初始化

### 1. 设置优先级分组

```c
/* 系统中统一设置一次，放在 NVIC_Init() 之前 */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
```

### 2. 清除 NVIC 旧挂起

```c
NVIC_ClearPendingIRQ(EXTI0_IRQn);
```

### 3. 配置并使能 EXTI0 中断通道

```c
NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

NVIC 不需要通过 RCC 开启时钟；中断函数由 CPU 自动进入，不在主程序中手动调用。

## 四、中断处理（以 EXTI0 为例）

### 1. 定义中断函数，判断并清除标志

```c
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        Key_Event = 1;
    }
}
```

### 2. 主循环处理按键事件

放在 `while (1)` 中；此例未加按键消抖。

```c
if (Key_Event != 0)
{
    Key_Event = 0;

    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == RESET)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    }
}
```

## 五、串口 USART1 初始化与收发

### 1. 开启 GPIOA、USART1 时钟

```c
RCC_APB2PeriphClockCmd(
    RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
    ENABLE
);
```

### 2. 配置 PA9 为 TX 复用推挽输出

```c
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
```

### 3. 配置 PA10 为 RX 浮空输入

```c
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOA, &GPIO_InitStructure);
```

### 4. 设置波特率、数据位、停止位、校验和收发模式

```c
USART_InitStructure.USART_BaudRate = 9600;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
USART_Init(USART1, &USART_InitStructure);
```

### 5. 开启 RXNE 接收中断请求

```c
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
```

### 6. 配置 USART1 的 NVIC 通道

```c
NVIC_ClearPendingIRQ(USART1_IRQn);

NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

### 7. 使能 USART1

```c
USART_Cmd(USART1, ENABLE);
```

### 8. 编写 USART1 接收中断函数

```c
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        /* 读取数据后，RXNE 自动清除 */
        USART1_RxData = (uint8_t)USART_ReceiveData(USART1);
    }
}
```

### 9. 编写发送一个字节的函数

```c
void USART1_SendByte(uint8_t Data)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

    USART_SendData(USART1, Data);
}
```

电脑端设为 9600、8N1；3.3 V USB 转串口模块的 RX 接 PA9、TX 接 PA10，并共地。

## 六、独立看门狗 IWDG 初始化

与 WWDG 分开练习；软件启动，分频 /64，重装载值 624，LSI=40 kHz 时约 1 秒。

### 1. 启动 LSI，等待就绪

```c
RCC_LSICmd(ENABLE);
while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
{
}
```

### 2. 允许写入预分频和重装载寄存器

```c
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
```

### 3. 设置 64 分频

```c
IWDG_SetPrescaler(IWDG_Prescaler_64);
```

### 4. 设置重装载值

```c
IWDG_SetReload(624);
```

### 5. 等待参数更新完成

```c
while ((IWDG_GetFlagStatus(IWDG_FLAG_PVU) != RESET) ||
       (IWDG_GetFlagStatus(IWDG_FLAG_RVU) != RESET))
{
}
```

### 6. 装入初值，启动 IWDG

```c
IWDG_ReloadCounter();
IWDG_Enable();
```

### 7. 正常任务中定期喂狗

```c
IWDG_ReloadCounter();
```

IWDG 无需配置 NVIC，也没有单独的 RCC 外设时钟使能项。

## 七、窗口看门狗 WWDG 初始化

与 IWDG 分开练习；/8 分频，窗口 0x5F，初值 0x7F，启用 EWI。NVIC 沿用第三部分的分组 2。

### 1. 开启 WWDG 时钟

```c
RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
```

### 2. 设置 8 分频

```c
WWDG_SetPrescaler(WWDG_Prescaler_8);
```

### 3. 设置窗口上限

```c
WWDG_SetWindowValue(0x5F);
```

### 4. 清除旧挂起

```c
WWDG_ClearFlag();
NVIC_ClearPendingIRQ(WWDG_IRQn);
```

### 5. 配置 WWDG 的 NVIC 通道

```c
NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

### 6. 开启 EWI 提前唤醒中断

```c
WWDG_EnableIT();
```

### 7. 装入初值，启动 WWDG

```c
WWDG_Enable(0x7F);
```

### 8. 编写窗口内喂狗函数，由正常任务调用

```c
void WWDG_Feed(void)
{
    uint32_t counter;

    counter = WWDG->CR & 0x7FU;
    if ((counter < 0x5FU) && (counter > 0x40U))
    {
        WWDG_SetCounter(0x7F);
    }
}
```

### 9. 编写 EWI 中断函数

```c
void WWDG_IRQHandler(void)
{
    if (WWDG_GetFlagStatus() != RESET)
    {
        WWDG_ClearFlag();
        WWDG_EarlyEvent = 1;
    }
}
```

喂狗应留出执行余量；PCLK1=36 MHz 时可选约 40 ms 的窗口内部时刻。EWI 示例只记录标志，未及时喂狗仍会复位。

## 八、通用定时器 TIM3 初始化与中断

独立实验：TIM3CLK=72 MHz，PSC=7199、ARR=4999，每 500 ms 中断一次。沿用第一部分 PB5 输出配置、第三部分 NVIC 分组 2。

### 1. 开启 TIM3 时钟

```c
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
```

### 2. 停止计数，选择内部时钟

```c
TIM_Cmd(TIM3, DISABLE);
TIM_InternalClockConfig(TIM3);
```

### 3. 填写并应用时基参数

```c
TIM_TimeBaseStructure.TIM_Prescaler = 7199;
TIM_TimeBaseStructure.TIM_Period = 4999;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
```

### 4. 设置计数初值

```c
TIM_SetCounter(TIM3, 0);
```

### 5. 清除更新标志和 NVIC 旧挂起

```c
TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
NVIC_ClearPendingIRQ(TIM3_IRQn);
```

### 6. 开启更新中断请求

```c
TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
```

### 7. 配置 TIM3 的 NVIC 通道

```c
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

### 8. 启动 TIM3

```c
TIM_Cmd(TIM3, ENABLE);
```

### 9. 编写 TIM3 中断函数

```c
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        TIM3_Event = 1;
    }
}
```

### 10. 在主循环处理事件，翻转 PB5

```c
if (TIM3_Event != 0)
{
    TIM3_Event = 0;

    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == RESET)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    }
}
```

周期公式：`T=(PSC+1)×(ARR+1)/TIM3CLK`。500 ms 翻转一次，完整亮灭周期为 1 秒。

参考：[ST RM0008 手册：GPIO、EXTI、USART、IWDG、WWDG、TIM](https://www.st.com/resource/en/reference_manual/rm0008-stm32f103xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)。
