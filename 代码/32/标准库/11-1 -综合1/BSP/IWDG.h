#ifndef __IWDG_H
#define __IWDG_H

#include "stm32f10x.h"

/* 独立看门狗
 * LSI 约 40kHz，分频 64 后计数频率约 625Hz，
 * 重装值 625 => 溢出时间约 1s。 */

#define IWDG_TIMEOUT_1S_PSC      IWDG_Prescaler_64
#define IWDG_TIMEOUT_1S_RELOAD   625

void Iwdg_Init(uint8_t IWDG_Prescaler, uint16_t Reload);

/* 直接配置成约 1s 超时，最常用 */
void Iwdg_Init_1s(void);

/* 喂狗：必须在 1s 内调用一次 */
void Eat_Dog(void);

#endif
