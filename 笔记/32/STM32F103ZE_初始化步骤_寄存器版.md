# STM32F103ZE 初始化步骤：寄存器版

[标准库版](C:/Users/27686/Desktop/嵌入式/笔记/32/STM32F103ZE_GPIO_NVIC初始化步骤详解.md) · [HAL 库版](C:/Users/27686/Desktop/嵌入式/笔记/32/STM32F103ZE_初始化步骤_HAL库版.md)

PA0 下拉按键；PB5 低电平亮；USART1 使用 PA9/PA10，9600、8N1。按键初始化顺序：GPIO → EXTI → NVIC。

使用芯片/CMSIS 头文件定义寄存器，不调用标准外设库初始化函数。变量放文件开头；中断和发送函数独立定义；已有同名定义时直接沿用。

## 准备：头文件与变量

```c
#include "stm32f10x.h"  /* 工程选择 STM32F10X_HD */

volatile uint8_t Key_Event = 0;
volatile uint8_t USART1_RxData = 0;
uint32_t PCLK2_Hz;
uint32_t APB2_DivCode;
volatile uint8_t WWDG_EarlyEvent = 0;
volatile uint8_t TIM3_Event = 0;
```

## 一、GPIO 初始化

### 1. 开启 GPIOA、GPIOB 时钟

```c
RCC->APB2ENR |= (1UL << 2) | (1UL << 3);
```

### 2. 设置 PB5 初始高电平，LED 熄灭

```c
GPIOB->BSRR = 1UL << 5;
```

### 3. 配置 PB5：推挽输出、2 MHz

```c
GPIOB->CRL &= ~(0xFUL << 20);
GPIOB->CRL |=  (0x2UL << 20);
```

### 4. 配置 PA0：上拉/下拉输入，选择下拉

```c
GPIOA->CRL &= ~0xFUL;
GPIOA->CRL |=  0x8UL;
GPIOA->BRR = 1UL << 0;  /* ODR0 = 0，选择下拉 */
```

## 二、EXTI 初始化

### 1. 开启 AFIO 时钟

```c
RCC->APB2ENR |= 1UL << 0;
```

### 2. 将 PA0 映射到 EXTI0

```c
AFIO->EXTICR[0] &= ~0xFUL;  /* EXTI0 端口选择 0000：GPIOA */
```

### 3. 配置 EXTI0：中断模式、上升沿

```c
EXTI->IMR  &= ~(1UL << 0);
EXTI->EMR  &= ~(1UL << 0);
EXTI->RTSR |=  (1UL << 0);
EXTI->FTSR &= ~(1UL << 0);
```

### 4. 清除旧挂起，并使能 EXTI0 中断请求

```c
EXTI->PR = 1UL << 0;  /* 写 1 清除，不能使用 |= */
EXTI->IMR |= 1UL << 0;
```

## 三、NVIC 初始化

### 1. 设置分组：2 位抢占、2 位子优先级

```c
/* PRIGROUP = 5，对应标准库和 HAL 的分组 2；系统统一设置一次 */
SCB->AIRCR = (SCB->AIRCR &
              ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk)) |
             (0x5FAUL << 16) | (5UL << 8);
```

### 2. 清除 EXTI0 的 NVIC 挂起

```c
/* EXTI0 的 IRQ 编号为 6 */
NVIC->ICPR[0] = 1UL << 6;
```

### 3. 设置 EXTI0 优先级：抢占 1、子优先级 1

```c
/* IP[] 是字节数组，4 位有效优先级放在高 4 位 */
NVIC->IP[6] = (uint8_t)(((1U << 2) | 1U) << 4);
```

### 4. 使能 EXTI0 中断通道

```c
NVIC->ISER[0] = 1UL << 6;
```

## 四、中断处理

### 1. 判断并清除 EXTI0 挂起，记录事件

```c
void EXTI0_IRQHandler(void)
{
    if ((EXTI->PR & (1UL << 0)) != 0U)
    {
        EXTI->PR = 1UL << 0;
        Key_Event = 1;
    }
}
```

### 2. 在主循环处理事件（未加消抖）

```c
if (Key_Event != 0)
{
    Key_Event = 0;

    if ((GPIOB->ODR & (1UL << 5)) != 0U)
    {
        GPIOB->BRR = 1UL << 5;
    }
    else
    {
        GPIOB->BSRR = 1UL << 5;
    }
}
```

## 五、串口 USART1 初始化与收发

沿用第三部分的优先级分组；系统时钟使用工程现有配置。

### 1. 开启 GPIOA、USART1 时钟

```c
RCC->APB2ENR |= (1UL << 2) | (1UL << 14);
```

### 2. 配置 PA9：复用推挽输出、50 MHz

```c
GPIOA->CRH &= ~(0xFUL << 4);
GPIOA->CRH |=  (0xBUL << 4);
```

### 3. 配置 PA10：浮空输入

```c
GPIOA->CRH &= ~(0xFUL << 8);
GPIOA->CRH |=  (0x4UL << 8);
```

### 4. 关闭 USART1，设置 8 数据位、无校验、1 停止位、无流控

```c
USART1->CR1 = 0;
USART1->CR2 = 0;
USART1->CR3 = 0;
```

### 5. 按当前 PCLK2 设置 9600 波特率

```c
SystemCoreClockUpdate();  /* CMSIS：更新当前 HCLK 频率变量 */
APB2_DivCode = (RCC->CFGR >> 11) & 0x7U;
PCLK2_Hz = SystemCoreClock;

if (APB2_DivCode >= 4U)
{
    PCLK2_Hz >>= (APB2_DivCode - 3U);
}

USART1->BRR = (PCLK2_Hz + 4800U) / 9600U;
```

### 6. 开启发送、接收和 RXNE 中断请求

```c
USART1->CR1 |= (1UL << 3) | (1UL << 2) | (1UL << 5);
```

### 7. 配置 USART1 的 NVIC 通道：抢占 2、子优先级 1

```c
/* USART1 IRQ = 37，位于第 1 组的第 5 位 */
NVIC->ICPR[1] = 1UL << 5;
NVIC->IP[37] = (uint8_t)(((2U << 2) | 1U) << 4);
NVIC->ISER[1] = 1UL << 5;
```

### 8. 使能 USART1

```c
USART1->CR1 |= 1UL << 13;
```

### 9. 编写接收中断函数

```c
void USART1_IRQHandler(void)
{
    if ((USART1->SR & (1UL << 5)) != 0U)
    {
        /* 读取 DR 后，RXNE 自动清除 */
        USART1_RxData = (uint8_t)USART1->DR;
    }
}
```

### 10. 发送一个字节

```c
void USART1_SendByte(uint8_t Data)
{
    while ((USART1->SR & (1UL << 7)) == 0U)
    {
    }

    USART1->DR = Data;
}
```

电脑端设为 9600、8N1；3.3 V USB 转串口模块 RX 接 PA9，TX 接 PA10，并共地。

## 六、独立看门狗 IWDG 初始化

与 WWDG 分开练习；软件启动，分频 /64，重装载值 624，LSI=40 kHz 时约 1 秒。

### 1. 启动 LSI，等待就绪

```c
RCC->CSR |= 1UL << 0;
while ((RCC->CSR & (1UL << 1)) == 0U)
{
}
```

### 2. 解锁 PR、RLR 写保护

```c
IWDG->KR = 0x5555U;
```

### 3. 设置 64 分频（PR 编码为 4）

```c
IWDG->PR = 4U;
```

### 4. 设置重装载值

```c
IWDG->RLR = 624U;
```

### 5. 等待 PVU、RVU 清零

```c
while ((IWDG->SR & 0x3U) != 0U)
{
}
```

### 6. 装入初值，启动 IWDG

```c
IWDG->KR = 0xAAAAU;
IWDG->KR = 0xCCCCU;
```

### 7. 正常任务中定期喂狗

```c
IWDG->KR = 0xAAAAU;
```

IWDG 无需配置 NVIC，也没有单独的 RCC 外设时钟使能项。

## 七、窗口看门狗 WWDG 初始化

与 IWDG 分开练习；/8 分频，窗口 0x5F，初值 0x7F，启用 EWI。NVIC 沿用第三部分的分组 2。

### 1. 开启 WWDG 时钟

```c
RCC->APB1ENR |= 1UL << 11;
```

### 2. 设置 8 分频、窗口上限

```c
/* WDGTB=3，对应 /8；W=0x5F */
WWDG->CFR = (3UL << 7) | 0x5FU;
```

### 3. 清除 EWI 和 NVIC 旧挂起

```c
WWDG->SR = 0;
NVIC->ICPR[0] = 1UL << 0;  /* WWDG 的 IRQ 编号为 0 */
```

### 4. 设置优先级，打开 NVIC 通道

```c
NVIC->IP[0] = 0;  /* 抢占优先级 0，子优先级 0 */
NVIC->ISER[0] = 1UL << 0;
```

### 5. 开启 EWI 提前唤醒中断

```c
WWDG->CFR |= 1UL << 9;
```

### 6. 装入初值，启动 WWDG

```c
WWDG->CR = (1UL << 7) | 0x7FU;  /* WDGA=1，T=0x7F */
```

### 7. 编写窗口内喂狗函数，由正常任务调用

```c
void WWDG_Feed(void)
{
    uint32_t counter;

    counter = WWDG->CR & 0x7FU;
    if ((counter < 0x5FU) && (counter > 0x40U))
    {
        WWDG->CR = (1UL << 7) | 0x7FU;
    }
}
```

### 8. 编写 EWI 中断函数

```c
void WWDG_IRQHandler(void)
{
    if ((WWDG->SR & (1UL << 0)) != 0U)
    {
        WWDG->SR = 0;  /* EWIF 写 0 清除 */
        WWDG_EarlyEvent = 1;
    }
}
```

喂狗应留出执行余量；PCLK1=36 MHz 时可选约 40 ms 的窗口内部时刻。EWI 示例只记录标志，未及时喂狗仍会复位。

## 八、通用定时器 TIM3 初始化与中断

独立实验：TIM3CLK=72 MHz，PSC=7199、ARR=4999，每 500 ms 中断一次。沿用第一部分 PB5 输出配置、第三部分 NVIC 分组 2。

### 1. 开启 TIM3 时钟

```c
RCC->APB1ENR |= 1UL << 1;
```

### 2. 停止计数，选择内部时钟、向上计数

```c
TIM3->CR1 = 0;
TIM3->SMCR = 0;
TIM3->DIER = 0;
```

### 3. 设置分频、重装载值和初始计数值

```c
TIM3->PSC = 7199;
TIM3->ARR = 4999;
TIM3->CNT = 0;
```

### 4. 产生更新事件，装入预分频参数

```c
TIM3->EGR = 1UL << 0;  /* UG=1 */
```

### 5. 清除更新标志和 NVIC 旧挂起

```c
TIM3->SR = (uint16_t)~(1U << 0);  /* UIF 写 0 清除 */
NVIC->ICPR[0] = 1UL << 29;       /* TIM3 IRQ 编号为 29 */
```

### 6. 配置 TIM3 的 NVIC 通道

```c
NVIC->IP[29] = (uint8_t)(((1U << 2) | 1U) << 4);
NVIC->ISER[0] = 1UL << 29;
```

### 7. 开启更新中断请求

```c
TIM3->DIER |= 1UL << 0;  /* UIE=1 */
```

### 8. 启动 TIM3

```c
TIM3->CR1 |= 1UL << 0;  /* CEN=1 */
```

### 9. 编写 TIM3 中断函数

```c
void TIM3_IRQHandler(void)
{
    if ((TIM3->SR & (1UL << 0)) != 0U)
    {
        TIM3->SR = (uint16_t)~(1U << 0);
        TIM3_Event = 1;
    }
}
```

### 10. 在主循环处理事件，翻转 PB5

```c
if (TIM3_Event != 0)
{
    TIM3_Event = 0;

    if ((GPIOB->ODR & (1UL << 5)) != 0U)
    {
        GPIOB->BRR = 1UL << 5;
    }
    else
    {
        GPIOB->BSRR = 1UL << 5;
    }
}
```

周期公式：`T=(PSC+1)×(ARR+1)/TIM3CLK`。500 ms 翻转一次，完整亮灭周期为 1 秒。

参考：[ST RM0008 手册](https://www.st.com/resource/en/reference_manual/rm0008-stm32f103xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)、[ST PM0056 内核手册](https://www.st.com/resource/en/programming_manual/cd00228163.pdf)。
