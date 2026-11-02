#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "key.h"

/* 必须根据开发板原理图设置：0 低电平工作，1 高电平工作。 */
#define LED1_ACTIVE_LEVEL  0U
#define LED2_ACTIVE_LEVEL  0U
#define BEEP_ACTIVE_LEVEL  1U

#define KEY_COUNT         4U
#define KEY_SCAN_MS       5U
#define BEEP_DURATION_MS  100U

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t active_level;
} Output_t;

typedef enum
{
    BEEP_OFF,
    BEEP_TIMED,
    BEEP_CONTINUOUS
} BeepMode_t;

static Output_t led1 = {GPIOB, GPIO_Pin_5, LED1_ACTIVE_LEVEL};
static Output_t led2 = {GPIOE, GPIO_Pin_5, LED2_ACTIVE_LEVEL};
static Output_t beep = {GPIOB, GPIO_Pin_8, BEEP_ACTIVE_LEVEL};
static Key_t keys[KEY_COUNT];
static volatile uint32_t tick_ms = 0;
static uint32_t app_now_ms = 0;
static BeepMode_t beep_mode = BEEP_OFF;
static uint32_t beep_started_ms = 0;

/* 如果工程已有 SysTick_Handler，请把 tick_ms++ 合并进去，只保留一个定义。 */
void SysTick_Handler(void)
{
    tick_ms++;
}

static void Output_Set(const Output_t *output, uint8_t on)
{
    uint8_t level;

    level = (on != 0U) ? output->active_level
                       : (uint8_t)!output->active_level;
    GPIO_WriteBit(output->port, output->pin, (BitAction)level);
}

/* 一个回调，通过 arg 可以控制不同的 LED。 */
static void Led_Toggle(void *arg)
{
    Output_t *led;
    uint8_t level;

    led = (Output_t *)arg;
    level = GPIO_ReadOutputDataBit(led->port, led->pin);
    GPIO_WriteBit(led->port, led->pin, (BitAction)!level);
}

static void AllLeds_On(void *arg)
{
    (void)arg;
    Output_Set(&led1, 1);
    Output_Set(&led2, 1);
}

static void AllLeds_Off(void *arg)
{
    (void)arg;
    Output_Set(&led1, 0);
    Output_Set(&led2, 0);
}

static void AllLeds_Toggle(void *arg)
{
    (void)arg;
    Led_Toggle(&led1);
    Led_Toggle(&led2);
}

/* 以下蜂鸣器功能以有源蜂鸣器/模块的使能控制为前提。 */
static void Beep_Once(void *arg)
{
    (void)arg;
    beep_mode = BEEP_TIMED;
    beep_started_ms = app_now_ms;
    Output_Set(&beep, 1);
}

static void Beep_ToggleContinuous(void *arg)
{
    (void)arg;
    if (beep_mode == BEEP_CONTINUOUS)
    {
        beep_mode = BEEP_OFF;
        Output_Set(&beep, 0);
    }
    else
    {
        beep_mode = BEEP_CONTINUOUS;
        Output_Set(&beep, 1);
    }
}

static void All_Off(void *arg)
{
    AllLeds_Off(arg);
    beep_mode = BEEP_OFF;
    Output_Set(&beep, 0);
}

static void Beep_Task(uint32_t now_ms)
{
    if ((beep_mode == BEEP_TIMED) &&
        ((uint32_t)(now_ms - beep_started_ms) >= BEEP_DURATION_MS))
    {
        beep_mode = BEEP_OFF;
        Output_Set(&beep, 0);
    }
}

static void App_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                          RCC_APB2Periph_GPIOB |
                          RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_StructInit(&gpio);

    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &gpio);

    /* 先设定关闭电平，再切换为输出，减少初始化时的误动作。 */
    All_Off(NULL);

    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOE, &gpio);
}

static void App_Keys_Init(void)
{
    /* 对象、端口、引脚、有效电平、消抖ms、长按ms、短按函数、长按函数、参数 */
    Key_Init(&keys[0], GPIOA, GPIO_Pin_0, 1, 20, 1000,
             Led_Toggle, AllLeds_On, &led1);

    Key_Init(&keys[1], GPIOE, GPIO_Pin_2, 0, 20, 1200,
             Led_Toggle, AllLeds_Off, &led2);

    Key_Init(&keys[2], GPIOE, GPIO_Pin_3, 0, 20, 1500,
             Beep_Once, Beep_ToggleContinuous, NULL);

    Key_Init(&keys[3], GPIOE, GPIO_Pin_4, 0, 20, 2000,
             AllLeds_Toggle, All_Off, NULL);
}

int main(void)
{
    uint32_t last_scan_ms;
    uint32_t i;

    /* 标准库启动文件通常已经调用 SystemInit。 */
    SystemCoreClockUpdate();
    App_GPIO_Init();
    App_Keys_Init();

    if (SysTick_Config(SystemCoreClock / 1000U) != 0U)
    {
        while (1)
        {
        }
    }

    last_scan_ms = tick_ms;
    while (1)
    {
        app_now_ms = tick_ms;

        if ((uint32_t)(app_now_ms - last_scan_ms) >= KEY_SCAN_MS)
        {
            last_scan_ms = app_now_ms;
            for (i = 0; i < KEY_COUNT; i++)
            {
                Key_Process(&keys[i], app_now_ms);
            }
        }

        Beep_Task(app_now_ms);
        /* 在这里继续处理其他非阻塞任务。 */
    }
}
