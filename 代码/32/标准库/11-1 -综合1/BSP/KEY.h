#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

/* ================== 按键连接（F103ZE） ==================
 * PA0  电源开关     （下拉输入，按下为高）
 * PB5  手动/自动切换 （下拉输入，按下为高）
 * PE5  定时关机     （下拉输入，按下为高）
 * PE4  亮度档位     （上拉输入，按下为低）
 * ====================================================== */

#define KEY_NUM        4
#define KEY_DEBOUNCE   2       /* 连续 2 次采样一致才算按下，2*10ms=20ms 消抖 */

#define KEY_POWER      1       /* PA0 */
#define KEY_MODE       2       /* PB5 */
#define KEY_TIMER      3       /* PE5 */
#define KEY_GEAR       4       /* PE4 */

void Key_Init(void);

/* 每 10ms 调用一次（由 TIM2 的 10ms 标志驱动），返回 0=无按键，否则返回按键号 */
uint8_t Key_Scan(void);

#endif
