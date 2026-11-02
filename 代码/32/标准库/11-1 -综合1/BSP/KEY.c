#include "KEY.h"

/* ============================================================
 * 简易按键扫描（非阻塞，由 TIM2 每 10ms 调一次）
 *
 * 1. 每 10ms 读一次电平，连续 2 次采样相同判定为稳定（20ms 消抖）
 * 2. 消抖完成后在"按下边沿"上报一次按键号，按住期间不重复上报
 * 3. 松手后恢复，允许下一次按下再上报
 * ============================================================ */

typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
	uint8_t active;			/* 按下时的电平 */
	uint8_t cnt;			/* 连续相同采样次数 */
	uint8_t last;			/* 上一次采样电平 */
	uint8_t pressed;		/* 本次按下是否已上报 */
} key_t;

static key_t s_key[KEY_NUM] =
{
	{ GPIOA, GPIO_Pin_0, 1, 0, 0, 0 },   /* PA0 下拉，按下为高 */
	{ GPIOB, GPIO_Pin_5, 1, 0, 0, 0 },   /* PB5 下拉，按下为高 */
	{ GPIOE, GPIO_Pin_5, 1, 0, 0, 0 },   /* PE5 下拉，按下为高 */
	{ GPIOE, GPIO_Pin_4, 0, 0, 0, 0 }    /* PE4 上拉，按下为低 */
};

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	uint8_t i;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* PA0 / PB5 / PE5：下拉输入 */
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitTypeStruct);
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitTypeStruct);
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitTypeStruct);

	/* PE4：上拉输入 */
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitTypeStruct);

	for (i = 0; i < KEY_NUM; i++)
	{
		s_key[i].cnt = 0;
		s_key[i].last = 0;
		s_key[i].pressed = 0;
	}
}

uint8_t Key_Scan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_NUM; i++)
	{
		key_t *k = &s_key[i];
		uint8_t now = (GPIO_ReadInputDataBit(k->port, k->pin) == k->active) ? 1 : 0;

		/* 连续相同采样计数 */
		if (now == k->last)
		{
			if (k->cnt < KEY_DEBOUNCE)
			{
				k->cnt++;
			}
		}
		else
		{
			k->last = now;
			k->cnt = 0;
		}

		/* 消抖完成且是刚按下的边沿：上报一次按键号 */
		if (k->cnt >= KEY_DEBOUNCE && now == 1 && k->pressed == 0)
		{
			k->pressed = 1;
			return (uint8_t)(i + 1);
		}

		/* 松手后允许下次再上报 */
		if (now == 0)
		{
			k->pressed = 0;
		}
	}

	return 0;
}
