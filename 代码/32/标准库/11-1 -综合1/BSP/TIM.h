#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

/* 系统节拍：TIM2 每 1ms 进入一次更新中断 */
extern volatile uint32_t g_tick_ms;			/* 上电以来的毫秒数 */
extern volatile uint32_t g_uptime_sec;		/* 上电以来的秒数 */
extern volatile uint8_t g_flag_10ms;		/* 10ms 标志，主循环用完清 0 */
extern volatile uint8_t g_flag_1s;			/* 1s 标志，主循环用完清 0 */

/* 配置 TIM2 为 1ms 节拍中断（72MHz 主频） */
void Tim2_Init_1ms(void);

/* 通用时基初始化：f = 72MHz / ((psc+1)*(arr+1)) */
void tim_init(u32 TIM_Period, u32 TIM_Prescaler);

uint32_t Tim_GetTick(void);

#endif
