#include "PWM.h"

/* ==========================================================
 * 主灯调光：TIM3_CH1 -> PA6（无需重映射）
 *
 * 计数时钟 = 72MHz / (PSC+1) = 72MHz / 720 = 100kHz
 * PWM 频率 = 100kHz / (ARR+1) = 100kHz / 100 = 1kHz
 * CCR = 0~100 直接对应 0%~100% 占空比
 *
 * 输出极性设为 High：外接 LED 高电平点亮，CCR 越大越亮。
 * 若你的灯是灌电流接法（低电平点亮），把极性改成 Low。
 * ========================================================== */

#define PWM_ARR      99
#define PWM_PSC      (720 - 1)

void pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitTypeStruct;

	/* 1. 时钟：GPIOA + TIM3 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* 2. PA6 复用推挽输出（TIM3_CH1） */
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitTypeStruct);

	/* 3. 时基 */
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = PWM_ARR;
	TIM_TimeBaseInitStruct.TIM_Prescaler = PWM_PSC;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	/* 4. 输出比较通道 1 */
	TIM_OCStructInit(&TIM_OCInitTypeStruct);
	TIM_OCInitTypeStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitTypeStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitTypeStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitTypeStruct.TIM_Pulse = 0;			/* 上电先灭灯 */
	TIM_OC1Init(TIM3, &TIM_OCInitTypeStruct);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* 5. 启动定时器 */
	TIM_Cmd(TIM3, ENABLE);
}

void pwm_set_duty(uint8_t percent)
{
	if (percent > PWM_MAX_DUTY)
	{
		percent = PWM_MAX_DUTY;
	}

	TIM_SetCompare1(TIM3, percent);
}

void pwm_setcompare2(uint16_t compare)
{
	if (compare > PWM_MAX_DUTY)
	{
		compare = PWM_MAX_DUTY;
	}

	TIM_SetCompare1(TIM3, compare);
}
