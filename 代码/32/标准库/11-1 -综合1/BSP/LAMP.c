#include "LAMP.h"
#include "PWM.h"
#include "GPIO.h"
#include "TIM.h"
#include "USART.h"

/* ==========================================================
 * 台灯应用层
 *
 * 状态机很简单：power(开关) x mode(手动/自动)，亮度目标值 s_bright
 * 每轮主循环把“目标亮度”同步到 PWM，只有变化时才写寄存器。
 *
 * 自动档说明：
 *   板子上没有接光敏传感器时，用一个 0~100 往返变化的“模拟环境光”
 *   代替 ADC 采样，用来演示“环境越暗、台灯越亮”的自动调光逻辑。
 *   以后接上光敏电阻(ADC)，只要把 lamp_read_ambient() 换成真实采样
 *   即可，其余逻辑不用改。
 * ========================================================== */

#define LAMP_REPORT_MS        5000		/* 自动上报周期 */
#define LAMP_AUTO_STEP_MS     200		/* 自动档调节周期 */
#define LAMP_AUTO_THRESHOLD   5			/* 亮度变化超过 5% 才重新调光 */
#define LAMP_DEFAULT_BRIGHT   70

static const uint8_t  s_gear_value[LAMP_GEAR_NUM] = { 0, 30, 70, 100 };
static const uint16_t s_timer_steps[4] = { 30, 60, 120, 0 };

static lamp_power_t s_power = LAMP_POWER_OFF;
static lamp_mode_t  s_mode = LAMP_MODE_MANUAL;

static uint8_t  s_bright = LAMP_DEFAULT_BRIGHT;	/* 目标亮度 0~100 */
static uint8_t  s_applied = 0;					/* 已经写入 PWM 的亮度 */

static uint16_t s_timer_set = 0;				/* 设定的定时秒数 */
static uint16_t s_timer_left = 0;				/* 剩余秒数 */
static uint32_t s_on_sec = 0;					/* 累计开机秒数 */

static uint32_t s_beep_off_ms = 0;
static uint8_t  s_beep_active = 0;
static uint8_t  s_alarm = 0;

static uint8_t  s_ambient = 0;					/* 模拟环境光 0~100 */
static uint8_t  s_ambient_dir = 1;				/* 1=变暗方向, 0=变亮方向 */
static uint32_t s_last_auto_ms = 0;
static uint32_t s_last_report_ms = 0;

/* 读取环境光：现在是模拟值，接光敏后改成 ADC 采样即可 */
static uint8_t lamp_read_ambient(void)
{
	return s_ambient;
}

static void lamp_auto_update(void)
{
	uint8_t target;
	uint8_t diff;

	if (s_mode != LAMP_MODE_AUTO || s_power != LAMP_POWER_ON)
	{
		return;
	}

	if ((Tim_GetTick() - s_last_auto_ms) < LAMP_AUTO_STEP_MS)
	{
		return;
	}

	s_last_auto_ms = Tim_GetTick();

	/* 模拟环境光：0% -> 100% -> 0%，约 40s 一个来回 */
	if (s_ambient_dir)
	{
		if (s_ambient >= 100)
		{
			s_ambient_dir = 0;
		}
		else
		{
			s_ambient++;
		}
	}
	else
	{
		if (s_ambient == 0)
		{
			s_ambient_dir = 1;
		}
		else
		{
			s_ambient--;
		}
	}

	/* 环境越暗，台灯越亮 */
	target = (uint8_t)(100 - lamp_read_ambient());
	diff = (target > s_bright) ? (uint8_t)(target - s_bright) : (uint8_t)(s_bright - target);

	if (diff >= LAMP_AUTO_THRESHOLD)
	{
		s_bright = target;
		Usart_Printf("[AUTO] 环境光=%u%%  自动亮度=%u%%\r\n",
		             (unsigned int)lamp_read_ambient(), (unsigned int)s_bright);
	}
}

void Lamp_Init(void)
{
	s_power = LAMP_POWER_OFF;
	s_mode = LAMP_MODE_MANUAL;
	s_bright = LAMP_DEFAULT_BRIGHT;
	s_applied = 0;
	s_timer_set = 0;
	s_timer_left = 0;
	s_on_sec = 0;
	s_beep_active = 0;
	s_alarm = 0;
	s_ambient = 0;
	s_ambient_dir = 1;
	s_last_auto_ms = Tim_GetTick();
	s_last_report_ms = Tim_GetTick();

	pwm_set_duty(0);
	BEEP_Off();
}

void Lamp_Task(void)
{
	uint32_t now = Tim_GetTick();

	/* 1. 蜂鸣提示超时关闭 */
	if (s_beep_active && (int32_t)(now - s_beep_off_ms) >= 0)
	{
		BEEP_Off();
		s_beep_active = 0;
	}

	/* 2. 自动档调光 */
	lamp_auto_update();

	/* 3. 1 秒任务：计时、定时关机、周期上报 */
	if (g_flag_1s)
	{
		g_flag_1s = 0;

		if (s_power == LAMP_POWER_ON)
		{
			s_on_sec++;
		}

		if (s_timer_left > 0)
		{
			s_timer_left--;

			if (s_timer_left == 0)
			{
				s_timer_set = 0;
				Lamp_SetPower(LAMP_POWER_OFF);
				Usart_Printf("[TMR] 定时时间到，台灯已自动关闭\r\n");
			}
		}

		if (s_power == LAMP_POWER_ON && (now - s_last_report_ms) >= LAMP_REPORT_MS)
		{
			s_last_report_ms = now;
			Lamp_PrintStatus();
		}
	}

	/* 4. 亮度同步到 PWM：只有目标值变化时才写，避免每圈都刷寄存器 */
	if (s_power == LAMP_POWER_ON)
	{
		if (s_bright != s_applied)
		{
			pwm_set_duty(s_bright);
			s_applied = s_bright;
		}
	}
	else if (s_applied != 0)
	{
		pwm_set_duty(0);
		s_applied = 0;
	}
}

void Lamp_SetPower(lamp_power_t on)
{
	if (on == LAMP_POWER_ON)
	{
		s_power = LAMP_POWER_ON;

		if (s_bright == 0)
		{
			s_bright = LAMP_DEFAULT_BRIGHT;
		}

		s_last_report_ms = Tim_GetTick();
	}
	else
	{
		s_power = LAMP_POWER_OFF;
		s_timer_set = 0;
		s_timer_left = 0;
	}
}

void Lamp_TogglePower(void)
{
	if (s_power == LAMP_POWER_ON)
	{
		Lamp_SetPower(LAMP_POWER_OFF);
		Usart_Printf("[EVT] 台灯关闭\r\n");
	}
	else
	{
		Lamp_SetPower(LAMP_POWER_ON);
		Usart_Printf("[EVT] 台灯开启，亮度 %u%%\r\n", (unsigned int)s_bright);
	}

	Lamp_Beep(60);
}

lamp_power_t Lamp_GetPower(void)
{
	return s_power;
}

void Lamp_SetMode(lamp_mode_t mode)
{
	s_mode = (mode == LAMP_MODE_AUTO) ? LAMP_MODE_AUTO : LAMP_MODE_MANUAL;

	if (s_mode == LAMP_MODE_AUTO)
	{
		/* 从当前亮度反推环境光，避免一切到自动档亮度就跳变 */
		s_ambient = (uint8_t)(100 - s_bright);
		s_ambient_dir = 1;
		s_last_auto_ms = Tim_GetTick();
		Usart_Printf("[MODE] 已切换到自动档（环境光自动调光）\r\n");
	}
	else
	{
		Usart_Printf("[MODE] 已切换到手动档\r\n");
	}
}

void Lamp_ToggleMode(void)
{
	Lamp_SetMode((s_mode == LAMP_MODE_AUTO) ? LAMP_MODE_MANUAL : LAMP_MODE_AUTO);
	Lamp_Beep(40);
}

lamp_mode_t Lamp_GetMode(void)
{
	return s_mode;
}

void Lamp_SetBrightness(uint8_t percent)
{
	if (percent > 100)
	{
		percent = 100;
	}

	if (s_mode == LAMP_MODE_AUTO)
	{
		s_mode = LAMP_MODE_MANUAL;
		Usart_Printf("[MODE] 收到手动调光指令，已退出自动档\r\n");
	}

	s_bright = percent;

	if (s_power != LAMP_POWER_ON)
	{
		Lamp_SetPower(LAMP_POWER_ON);
	}
}

uint8_t Lamp_GetBrightness(void)
{
	return s_bright;
}

void Lamp_SetGear(uint8_t index)
{
	if (index >= LAMP_GEAR_NUM)
	{
		return;
	}

	s_bright = s_gear_value[index];

	if (s_power != LAMP_POWER_ON)
	{
		Lamp_SetPower(LAMP_POWER_ON);
	}

	Usart_Printf("[GEAR] 档位=%u  亮度=%u%%\r\n",
	             (unsigned int)index, (unsigned int)s_bright);
}

void Lamp_NextGear(void)
{
	uint8_t i;
	uint8_t idx = 0xFF;

	/* 当前亮度若正好是某一档，就切到下一档；否则从第 0 档开始 */
	for (i = 0; i < LAMP_GEAR_NUM; i++)
	{
		if (s_gear_value[i] == s_bright)
		{
			idx = i;
			break;
		}
	}

	idx = (idx == 0xFF) ? 0 : (uint8_t)((idx + 1) % LAMP_GEAR_NUM);

	Lamp_SetGear(idx);
	Lamp_Beep(40);
}

void Lamp_SetOffTimer(uint16_t seconds)
{
	if (seconds > 3600)
	{
		seconds = 3600;
	}

	s_timer_set = seconds;
	s_timer_left = seconds;

	if (seconds == 0)
	{
		Usart_Printf("[TMR] 定时关机已取消\r\n");
	}
	else
	{
		Usart_Printf("[TMR] 定时关机已设定：%u 秒后关闭\r\n", (unsigned int)seconds);
	}
}

void Lamp_CycleOffTimer(void)
{
	uint16_t next = 30;
	uint8_t i;

	for (i = 0; i < 4; i++)
	{
		if (s_timer_set == s_timer_steps[i])
		{
			next = s_timer_steps[(i + 1) % 4];
			break;
		}
	}

	Lamp_SetOffTimer(next);
	Lamp_Beep(40);
}

uint16_t Lamp_GetOffTimer(void)
{
	return s_timer_left;
}

uint32_t Lamp_GetOnTime(void)
{
	return s_on_sec;
}

void Lamp_Beep(uint16_t ms)
{
	if (s_alarm)
	{
		return;			/* 报警长鸣优先 */
	}

	s_beep_off_ms = Tim_GetTick() + ms;
	s_beep_active = 1;
	BEEP_On();
}

void Lamp_SetAlarm(uint8_t on)
{
	s_alarm = on ? 1 : 0;

	if (s_alarm)
	{
		BEEP_On();
		s_beep_active = 0;
	}
	else
	{
		BEEP_Off();
	}

	Usart_Printf("[ALM] %s\r\n",
	             s_alarm ? "故障报警：蜂鸣器长鸣（模拟过温保护）" : "报警解除");
}

uint8_t Lamp_GetAlarm(void)
{
	return s_alarm;
}

void Lamp_PrintStatus(void)
{
	Usart_Printf("[STATUS] PWR=%s MODE=%s BRIGHT=%u%% TIMER=%us ONTIME=%us UPTIME=%us ALM=%u\r\n",
	             (s_power == LAMP_POWER_ON) ? "ON" : "OFF",
	             (s_mode == LAMP_MODE_AUTO) ? "AUTO" : "MANUAL",
	             (unsigned int)s_bright,
	             (unsigned int)s_timer_left,
	             (unsigned int)s_on_sec,
	             (unsigned int)g_uptime_sec,
	             (unsigned int)s_alarm);
}
