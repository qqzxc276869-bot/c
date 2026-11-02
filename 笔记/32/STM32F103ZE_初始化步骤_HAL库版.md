# STM32F103ZE 初始化步骤：HAL 库版

[标准库版](C:/Users/27686/Desktop/嵌入式/笔记/32/STM32F103ZE_GPIO_NVIC初始化步骤详解.md) · [寄存器版](C:/Users/27686/Desktop/嵌入式/笔记/32/STM32F103ZE_初始化步骤_寄存器版.md)

PA0 下拉按键；PB5 低电平亮；USART1 使用 PA9/PA10，9600、8N1。按键初始化顺序：GPIO → EXTI → NVIC。

变量放文件开头；初始化语句放初始化函数；中断、回调、发送函数独立定义。CubeMX 已生成的变量、MSP、函数和回调直接沿用或合并。

## 准备：头文件与变量

```c
#include "stm32f1xx_hal.h"

GPIO_InitTypeDef GPIO_InitStructure = {0};
UART_HandleTypeDef huart1 = {0};
IWDG_HandleTypeDef hiwdg = {0};
WWDG_HandleTypeDef hwwdg = {0};
TIM_HandleTypeDef htim3 = {0};
TIM_ClockConfigTypeDef TIM_ClockConfigStructure = {0};

volatile uint8_t Key_Event = 0;
uint8_t USART1_RxBuffer = 0;
volatile uint8_t USART1_RxData = 0;

void SystemClock_Config(void);  /* 使用工程已有的系统时钟配置函数 */
volatile uint8_t WWDG_EarlyEvent = 0;
volatile uint8_t TIM3_Event = 0;
```

### 首先：在 main() 中初始化 HAL 和系统时钟

```c
HAL_Init();
SystemClock_Config();
```

### 保留工程中的 SysTick 中断计时

```c
void SysTick_Handler(void)
{
    HAL_IncTick();
}
```

## 一、GPIO 初始化

### 1. 开启 GPIOA、GPIOB 时钟

```c
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOB_CLK_ENABLE();
```

### 2. 设置 PB5 初始高电平，LED 熄灭

```c
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
```

### 3. 配置 PB5 推挽输出

```c
GPIO_InitStructure.Pin = GPIO_PIN_5;
GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStructure.Pull = GPIO_NOPULL;
GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
```

### 4. 配置 PA0 下拉输入

```c
GPIO_InitStructure.Pin = GPIO_PIN_0;
GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
GPIO_InitStructure.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
```

## 二、EXTI 初始化

### 1. 开启 AFIO 时钟

```c
__HAL_RCC_AFIO_CLK_ENABLE();
```

### 2. 配置 PA0 下拉、上升沿中断及 EXTI0 映射

```c
GPIO_InitStructure.Pin = GPIO_PIN_0;
GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
GPIO_InitStructure.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
```

### 3. 清除 EXTI0 旧挂起

```c
__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
```

## 三、NVIC 初始化

### 1. 设置优先级分组：2 位抢占、2 位子优先级

```c
HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
```

### 2. 清除旧挂起

```c
HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
```

### 3. 设置 EXTI0 优先级

```c
HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 1);
```

### 4. 使能 EXTI0 中断通道

```c
HAL_NVIC_EnableIRQ(EXTI0_IRQn);
```

## 四、中断处理

### 1. 中断入口调用 HAL 处理函数

```c
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
```

### 2. 在回调中记录按键事件

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0)
    {
        Key_Event = 1;
    }
}
```

### 3. 在主循环处理事件（未加消抖）

```c
if (Key_Event != 0)
{
    Key_Event = 0;
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}
```

## 五、串口 USART1 初始化与收发

沿用第三部分的优先级分组；GPIO、时钟代码可放入已有的 `HAL_UART_MspInit()`。

### 1. 开启 GPIOA、USART1 时钟

```c
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_USART1_CLK_ENABLE();
```

### 2. 配置 PA9 为 TX 复用推挽输出

```c
GPIO_InitStructure.Pin = GPIO_PIN_9;
GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
GPIO_InitStructure.Pull = GPIO_NOPULL;
GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
```

### 3. 配置 PA10 为 RX 输入

```c
GPIO_InitStructure.Pin = GPIO_PIN_10;
GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
GPIO_InitStructure.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
```

### 4. 配置并初始化 USART1

```c
huart1.Instance = USART1;
huart1.Init.BaudRate = 9600;
huart1.Init.WordLength = UART_WORDLENGTH_8B;
huart1.Init.StopBits = UART_STOPBITS_1;
huart1.Init.Parity = UART_PARITY_NONE;
huart1.Init.Mode = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
huart1.Init.OverSampling = UART_OVERSAMPLING_16;
HAL_UART_Init(&huart1);
```

### 5. 配置 USART1 的 NVIC 通道

```c
HAL_NVIC_ClearPendingIRQ(USART1_IRQn);
HAL_NVIC_SetPriority(USART1_IRQn, 2, 1);
HAL_NVIC_EnableIRQ(USART1_IRQn);
```

### 6. 启动一个字节的中断接收

```c
HAL_UART_Receive_IT(&huart1, &USART1_RxBuffer, 1);
```

### 7. 中断入口调用 HAL 串口处理函数

```c
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}
```

### 8. 接收完成后保存数据，并再次启动接收

```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        USART1_RxData = USART1_RxBuffer;
        HAL_UART_Receive_IT(&huart1, &USART1_RxBuffer, 1);
    }
}
```

### 9. 发送一个字节

```c
void USART1_SendByte(uint8_t Data)
{
    HAL_UART_Transmit(&huart1, &Data, 1, 100);
}
```

电脑端设为 9600、8N1；3.3 V USB 转串口模块 RX 接 PA9，TX 接 PA10，并共地。

## 六、独立看门狗 IWDG 初始化

与 WWDG 分开练习；分频 /64，重装载值 624，LSI=40 kHz 时约 1 秒。沿用前文的 HAL 和系统时钟初始化。

### 准备：确认 stm32f1xx_hal_conf.h 已启用模块

```c
#define HAL_IWDG_MODULE_ENABLED
#define HAL_WWDG_MODULE_ENABLED
```

### 1. 选择 IWDG 实例

```c
hiwdg.Instance = IWDG;
```

### 2. 设置 64 分频

```c
hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
```

### 3. 设置重装载值

```c
hiwdg.Init.Reload = 624;
```

### 4. 初始化并启动 IWDG

```c
HAL_IWDG_Init(&hiwdg);
```

该函数会启动 IWDG/LSI、设置参数、等待更新并首次喂狗；无需另配 NVIC。

### 5. 正常任务中定期喂狗

```c
HAL_IWDG_Refresh(&hiwdg);
```

## 七、窗口看门狗 WWDG 初始化

与 IWDG 分开练习；/8 分频，窗口 0x5F，初值 0x7F，启用 EWI。NVIC 沿用第三部分的分组 2。

### 1. 开启 WWDG 时钟

```c
__HAL_RCC_WWDG_CLK_ENABLE();
```

### 2. 选择 WWDG 实例

```c
hwwdg.Instance = WWDG;
```

### 3. 填写分频、窗口、初值及 EWI 参数

```c
hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
hwwdg.Init.Window = 0x5F;
hwwdg.Init.Counter = 0x7F;
hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
```

### 4. 清旧挂起，配置 WWDG 的 NVIC 通道

```c
__HAL_WWDG_CLEAR_FLAG(&hwwdg, WWDG_FLAG_EWIF);
HAL_NVIC_ClearPendingIRQ(WWDG_IRQn);
HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
HAL_NVIC_EnableIRQ(WWDG_IRQn);
```

### 5. 初始化并启动 WWDG

```c
HAL_WWDG_Init(&hwwdg);
```

### 6. 编写窗口内喂狗函数，由正常任务调用

```c
void WWDG_Feed(void)
{
    uint32_t counter;

    counter = hwwdg.Instance->CR & 0x7FU;
    if ((counter < 0x5FU) && (counter > 0x40U))
    {
        HAL_WWDG_Refresh(&hwwdg);
    }
}
```

### 7. 中断入口调用 HAL 处理函数

```c
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&hwwdg);
}
```

### 8. 在 EWI 回调中记录事件

```c
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    if (hwwdg->Instance == WWDG)
    {
        WWDG_EarlyEvent = 1;
    }
}
```

喂狗应留出执行余量；PCLK1=36 MHz 时可选约 40 ms 的窗口内部时刻。HAL 中断处理函数会清 EWIF；本回调只记录事件，未及时喂狗仍会复位。

## 八、通用定时器 TIM3 初始化与中断

独立实验：TIM3CLK=72 MHz，PSC=7199、ARR=4999，每 500 ms 中断一次。沿用第一部分 PB5 输出配置、第三部分 NVIC 分组 2。

### 准备：确认 stm32f1xx_hal_conf.h 已启用 TIM

```c
#define HAL_TIM_MODULE_ENABLED
```

### 1. 开启 TIM3 时钟

```c
__HAL_RCC_TIM3_CLK_ENABLE();
```

### 2. 填写并应用时基参数

```c
htim3.Instance = TIM3;
htim3.Init.Prescaler = 7199;
htim3.Init.Period = 4999;
htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
htim3.Init.RepetitionCounter = 0;
htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
HAL_TIM_Base_Init(&htim3);
```

### 3. 选择内部时钟

```c
TIM_ClockConfigStructure.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
HAL_TIM_ConfigClockSource(&htim3, &TIM_ClockConfigStructure);
```

### 4. 设置计数初值，清除更新标志

```c
__HAL_TIM_SET_COUNTER(&htim3, 0);
__HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
```

### 5. 配置 TIM3 的 NVIC 通道

```c
HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);
HAL_NVIC_EnableIRQ(TIM3_IRQn);
```

### 6. 开启更新中断并启动 TIM3

```c
HAL_TIM_Base_Start_IT(&htim3);
```

### 7. 中断入口调用 HAL 处理函数

```c
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim3);
}
```

### 8. 在周期回调中记录事件

```c
/* 已有同名回调时，将 TIM3 分支合并进去 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        TIM3_Event = 1;
    }
}
```

### 9. 在主循环处理事件，翻转 PB5

```c
if (TIM3_Event != 0)
{
    TIM3_Event = 0;
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}
```

周期公式：`T=(PSC+1)×(ARR+1)/TIM3CLK`。HAL 中断处理函数会清更新标志；500 ms 翻转一次，完整亮灭周期为 1 秒。

参考：[ST HAL GPIO 源码](https://github.com/STMicroelectronics/stm32f1xx-hal-driver/blob/master/Src/stm32f1xx_hal_gpio.c)、[ST HAL UART 源码](https://github.com/STMicroelectronics/stm32f1xx-hal-driver/blob/master/Src/stm32f1xx_hal_uart.c)、[ST HAL NVIC 源码](https://github.com/STMicroelectronics/stm32f1xx-hal-driver/blob/master/Src/stm32f1xx_hal_cortex.c)。

看门狗参考：[ST HAL IWDG](https://github.com/STMicroelectronics/stm32f1xx-hal-driver/blob/master/Src/stm32f1xx_hal_iwdg.c)、[ST HAL WWDG](https://github.com/STMicroelectronics/stm32f1xx-hal-driver/blob/master/Src/stm32f1xx_hal_wwdg.c)。

定时器参考：[ST HAL TIM 源码](https://github.com/STMicroelectronics/stm32f1xx-hal-driver/blob/master/Src/stm32f1xx_hal_tim.c)。
