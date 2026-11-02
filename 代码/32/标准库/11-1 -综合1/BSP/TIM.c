#include "TIM.h"
#include "GPIO.h"

/* ==========================================================
 * TIM2：系统节拍定时器
 *
 * 72MHz / (7199+1) = 10kHz，再数 10 个 => 1kHz => 1ms 中断一次。
 * 在中断里完成三件事：
 *   1. 毫秒计数 g_tick_ms
 *   2. 每 10ms 置一次 g_flag_10ms  -> 主循环用它做按键扫描
 *   3. 每 1s 置一次 g_flag_1s      -> 心跳灯翻转 + 定时关机倒计时
 *
 * 心跳灯(PB0)放在中断里翻转，即使主循环卡死，心跳灯依旧会闪，
 * 这样就能直观区分“程序卡死但中断还在跑”和“真正复位重启”。
 * ========================================================== */

volatile uint32_t g_tick_ms = 0;
volatile uint32_t g_uptime_sec = 0;
volatile uint8_t g_flag_10ms = 0;
volatile uint8_t g_flag_1s = 0;

static volatile uint16_t s_cnt_10ms = 0;
static volatile uint16_t s_cnt_1s = 0;

void tim_init(u32 TIM_Period, u32 TIM_Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InternalClockConfig(TIM2);

	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = TIM_Period;
	TIM_TimeBaseInitStruct.TIM_Prescaler = TIM_Prescaler;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM2, ENABLE);
}

void Tim2_Init_1ms(void)
{
	tim_init(9, 7199);
}

uint32_t Tim_GetTick(void)
{
	return g_tick_ms;
}

/* 中断服务函数：只做最轻量的计数和置标志，绝不在这里调用 printf */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		g_tick_ms++;

		if (++s_cnt_10ms >= 10)
		{
			s_cnt_10ms = 0;
			g_flag_10ms = 1;
		}

		if (++s_cnt_1s >= 1000)
		{
			s_cnt_1s = 0;
			g_uptime_sec++;
			g_flag_1s = 1;
			HeartLED_Toggle();		/* 心跳灯：1Hz 闪烁 */
		}
	}
}
