#include "GPIO.h"

/* ==================== 输出口初始化 ====================
 * 只配置 HEART_LED(PB0) 与 BEEP(PB8)。
 * 按键（PB5/PE5/PA0/PE4）是输入口，由 KEY.c 配置；
 * 台灯 PWM（PA6/TIM3_CH1）由 pwm_init() 配成复用推挽，这里不能动。
 * ====================================================== */
void gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitTypeStruct.GPIO_Pin = BEEP_PIN;			/* BEEP */
	GPIO_Init(BEEP_PORT, &GPIO_InitTypeStruct);

	GPIO_InitTypeStruct.GPIO_Pin = HEART_LED_PIN;		/* 心跳灯 */
	GPIO_Init(HEART_LED_PORT, &GPIO_InitTypeStruct);

	/* 上电默认：心跳灯灭、蜂鸣器不响 */
	GPIO_WriteBit(HEART_LED_PORT, HEART_LED_PIN, (BitAction)LED_LEVEL_OFF);
	GPIO_WriteBit(BEEP_PORT, BEEP_PIN, (BitAction)BEEP_LEVEL_OFF);
}

void HeartLED_On(void)
{
	GPIO_WriteBit(HEART_LED_PORT, HEART_LED_PIN, (BitAction)LED_LEVEL_ON);
}

void HeartLED_Off(void)
{
	GPIO_WriteBit(HEART_LED_PORT, HEART_LED_PIN, (BitAction)LED_LEVEL_OFF);
}

void HeartLED_Toggle(void)
{
	if (GPIO_ReadOutputDataBit(HEART_LED_PORT, HEART_LED_PIN) == Bit_SET)
	{
		HeartLED_Off();
	}
	else
	{
		HeartLED_On();
	}
}

void BEEP_On(void)
{
	GPIO_WriteBit(BEEP_PORT, BEEP_PIN, (BitAction)BEEP_LEVEL_ON);
}

void BEEP_Off(void)
{
	GPIO_WriteBit(BEEP_PORT, BEEP_PIN, (BitAction)BEEP_LEVEL_OFF);
}

void BEEP_Toggle(void)
{
	if (GPIO_ReadOutputDataBit(BEEP_PORT, BEEP_PIN) == Bit_SET)
	{
		BEEP_Off();
	}
	else
	{
		BEEP_On();
	}
}

void Led_Turn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)));
}

void Beep_Turn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)));
}
