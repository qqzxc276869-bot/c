#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

/* TIM3_CH1 -> PA6，1kHz PWM，占空比 0~100%
 * 台灯主灯由 PA6 驱动（高电平点亮）；若你的灯是灌电流接法（低电平点亮），
 * 把 PWM.c 里的 TIM_OCPolarity_High 改成 TIM_OCPolarity_Low 即可。 */
#define PWM_MAX_DUTY   100

void pwm_init(void);

/* 占空比 0~100，直接对应亮度百分比 */
void pwm_set_duty(uint8_t percent);

/* 兼容旧例程接口，参数同样为 0~100 */
void pwm_setcompare2(uint16_t compare);

#endif
