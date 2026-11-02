#ifndef __LAMP_H
#define __LAMP_H

#include "stm32f10x.h"

/* ================= 台灯应用层 =================
 * 负责：开关机、手动档位、自动档、定时关机、状态上报、蜂鸣提示
 * 只调用驱动层接口，不直接操作寄存器，方便移植。
 * ============================================== */

#define LAMP_GEAR_NUM   4

typedef enum
{
	LAMP_POWER_OFF = 0,
	LAMP_POWER_ON  = 1
} lamp_power_t;

typedef enum
{
	LAMP_MODE_MANUAL = 0,		/* 手动档：按键/串口指定亮度 */
	LAMP_MODE_AUTO   = 1		/* 自动档：按环境光自动调光（未接光敏时用模拟量） */
} lamp_mode_t;

void Lamp_Init(void);

/* 主循环里一直调用：处理蜂鸣超时、1s 定时、自动档调节、PWM 输出同步 */
void Lamp_Task(void);

void Lamp_TogglePower(void);
void Lamp_SetPower(lamp_power_t on);
lamp_power_t Lamp_GetPower(void);

void Lamp_ToggleMode(void);
void Lamp_SetMode(lamp_mode_t mode);
lamp_mode_t Lamp_GetMode(void);

void Lamp_SetBrightness(uint8_t percent);
uint8_t Lamp_GetBrightness(void);

void Lamp_SetGear(uint8_t index);		/* 0~3 对应 0% / 30% / 70% / 100% */
void Lamp_NextGear(void);

void Lamp_SetOffTimer(uint16_t seconds);	/* 0 表示取消定时 */
void Lamp_CycleOffTimer(void);				/* 30s -> 60s -> 120s -> 取消 */
uint16_t Lamp_GetOffTimer(void);

uint32_t Lamp_GetOnTime(void);

void Lamp_Beep(uint16_t ms);				/* 非阻塞蜂鸣提示 */
void Lamp_SetAlarm(uint8_t on);				/* 故障报警：蜂鸣器长鸣 */
uint8_t Lamp_GetAlarm(void);

void Lamp_PrintStatus(void);

#endif
