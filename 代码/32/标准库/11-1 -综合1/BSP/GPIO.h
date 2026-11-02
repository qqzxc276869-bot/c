#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"
#include "sys.h"

/* ================== 台灯控制系统 硬件连接（F103ZE） ==================
 * KEY_MODE  PB5  模式切换键（手动/自动），下拉输入，按下为高
 * KEY_TIMER PE5  定时关机键，下拉输入，按下为高
 * KEY_POWER PA0  开关机键，下拉输入，按下为高
 * KEY_GEAR  PE4  亮度档位键，上拉输入，按下为低
 * BEEP      PB8  蜂鸣器，高电平响
 * HEART_LED PB0  心跳指示灯，低电平点亮（1s 闪烁，由 TIM2 翻转）
 * LAMP_PWM  PA6  台灯主灯，TIM3_CH1 输出 PWM 无级调光（由 PWM.c 配置）
 *
 * 说明：原板载 LED PB5/PE5 已改作按键，心跳灯请外接一个 LED 到 PB0；
 *       若你的板子上心跳灯在别的引脚，改下面两个宏即可。
 * ========================================================== */

#define HEART_LED_PORT  GPIOB
#define HEART_LED_PIN   GPIO_Pin_0

#define BEEP_PORT       GPIOB
#define BEEP_PIN        GPIO_Pin_8

/* 板载 LED 为灌电流驱动：引脚输出低电平点亮 */
#define LED_LEVEL_ON    0
#define LED_LEVEL_OFF   1

/* 蜂鸣器为高电平驱动；若你的板子是低电平驱动，把这两个宏互换即可 */
#define BEEP_LEVEL_ON   1
#define BEEP_LEVEL_OFF  0

void gpio_Init(void);

void HeartLED_On(void);
void HeartLED_Off(void);
void HeartLED_Toggle(void);

void BEEP_On(void);
void BEEP_Off(void);
void BEEP_Toggle(void);

/* 兼容旧例程的通用翻转接口 */
void Led_Turn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Beep_Turn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif
