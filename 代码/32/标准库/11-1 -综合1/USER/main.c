#include "main.h"
#include "string.h"

/* ==========================================================
 * 台灯控制系统（简易学习版）
 *
 * 外设：GPIO 按键+蜂鸣器 / PWM 调光 / TIM2 定时 / USART1 / IWDG
 *
 * 按键：
 *   PA0 电源开关      PB5 手动/自动     PE5 定时关机(10s)   PE4 亮度档位
 *   （PA0/PB5/PE5 下拉输入按下为高；PE4 上拉输入按下为低）
 *
 * 串口（115200，命令大写）：
 *   SET_SPEED:50   设置亮度 0~100
 *   STATUS         打印状态
 *   ON / OFF       开机 / 关机
 *   HANG           进入死循环，演示看门狗约 1s 后自动复位
 *
 * 看门狗：主循环末尾喂狗；喂狗绝不能放中断里，否则主循环卡死
 *         也照样喂狗，看门狗就失去意义了。
 * ========================================================== */

static uint8_t  s_power = 0;	/* 0=关 1=开 */
static uint8_t  s_auto = 0;		/* 0=手动 1=自动(呼吸灯) */
static uint8_t  s_bright = 0;	/* 当前亮度 0~100 */
static uint16_t s_timer = 0;	/* 定时关机剩余秒数，0=未启用 */
static uint32_t s_ontime = 0;	/* 本次开机累计秒数 */
static uint32_t s_beep_ms = 0;	/* 蜂鸣结束时间点 */
static uint32_t s_last_auto = 0;/* 自动档上次调节时间点 */
static uint8_t  s_dir = 1;		/* 自动档亮度方向：1=变亮 0=变暗 */

static const uint8_t s_gear[4] = { 0, 30, 70, 100 };

/* 蜂鸣提示（非阻塞，到点自动关） */
static void beep(uint16_t ms)
{
	BEEP_On();
	s_beep_ms = Tim_GetTick() + ms;
}

static void set_bright(uint8_t v)
{
	if (v > 100)
	{
		v = 100;
	}

	s_bright = v;
	pwm_set_duty(v);
}

static void print_status(void)
{
	Usart_Printf("[状态] 电源=%s 模式=%s 亮度=%u%% 定时=%us 开机=%us\r\n",
	             s_power ? "开" : "关",
	             s_auto ? "自动" : "手动",
	             (unsigned int)s_bright,
	             (unsigned int)s_timer,
	             (unsigned int)s_ontime);
}

static void power_off(void)
{
	s_power = 0;
	s_timer = 0;
	pwm_set_duty(0);
	Usart_Printf("[系统] 已关机\r\n");
}

/* ---------------- 串口命令 ---------------- */

static void cmd_exec(char *line)
{
	uint32_t v = 0;
	char *p;

	if (strncmp(line, "SET_SPEED:", 10) == 0)
	{
		p = line + 10;
		while (*p >= '0' && *p <= '9')
		{
			v = v * 10 + (uint32_t)(*p - '0');
			p++;
		}

		if (v <= 100)
		{
			s_auto = 0;				/* 手动调光时退出自动档 */
			set_bright((uint8_t)v);
			Usart_Printf("[串口] 亮度设为 %u%%\r\n", (unsigned int)v);
		}
		else
		{
			Usart_Printf("[串口] 亮度范围 0~100\r\n");
		}
	}
	else if (strcmp(line, "STATUS") == 0)
	{
		print_status();
	}
	else if (strcmp(line, "ON") == 0)
	{
		s_power = 1;
		Usart_Printf("[串口] 已开机\r\n");
	}
	else if (strcmp(line, "OFF") == 0)
	{
		power_off();
		Usart_Printf("[串口] 已关机\r\n");
	}
	else if (strcmp(line, "HANG") == 0)
	{
		/* 故意死循环且不喂狗：观察系统是否被看门狗复位 */
		Usart_Printf("[串口] 进入死循环，约 1s 后看门狗应复位系统...\r\n");
		while (1)
		{
		}
	}
	else
	{
		Usart_Printf("[串口] 未知命令，可用：SET_SPEED:50  STATUS  ON  OFF  HANG\r\n");
	}
}

/* ---------------- 主程序 ---------------- */

int main(void)
{
	uint8_t key;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 初始化 */
	delay_init();
	Usart_All_Init(115200);		/* 串口最先初始化，启动日志才不会丢 */
	gpio_Init();				/* 心跳灯 PB0 / 蜂鸣器 PB8 */
	Key_Init();					/* PA0 / PB5 / PE5 / PE4 */
	pwm_init();					/* TIM3_CH1 -> PA6，1kHz */
	Tim2_Init_1ms();			/* 1ms 节拍：10ms 按键标志 / 1s 心跳与定时 */
	Iwdg_Init_1s();				/* 独立看门狗，主循环末尾喂狗 */

	Usart_Printf("\r\n=== 台灯控制系统（简易版）===\r\n");
	Usart_Printf("按键: PA0电源  PB5模式  PE5定时  PE4亮度\r\n");
	Usart_Printf("串口: SET_SPEED:50  STATUS  ON  OFF  HANG\r\n");

	/* 上电判断是不是被看门狗复位的 */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		Usart_Printf("[上电] 复位原因：独立看门狗(IWDG)复位\r\n");
		RCC_ClearFlag();
	}

	while (1)
	{
		/* 1. 串口命令 */
		{
			char line[64];

			if (Usart_GetLine(line, sizeof(line)))
			{
				cmd_exec(line);
			}
		}

		/* 2. 按键（10ms 标志驱动，KEY_Scan 内部做 20ms 消抖） */
		if (g_flag_10ms)
		{
			g_flag_10ms = 0;
			key = Key_Scan();

			if (key == KEY_POWER)			/* PA0：开关机 */
			{
				if (s_power)
				{
					power_off();
					Usart_Printf("[按键] 已关机\r\n");
				}
				else
				{
					s_power = 1;
					Usart_Printf("[按键] 已开机，亮度 %u%%\r\n", (unsigned int)s_bright);
				}

				beep(60);
			}
			else if (key == KEY_MODE)		/* PB5：手动/自动 */
			{
				s_auto = s_auto ? 0 : 1;
				if (s_auto)
				{
					s_last_auto = Tim_GetTick();
				}

				Usart_Printf("[按键] 模式：%s\r\n", s_auto ? "自动(呼吸灯)" : "手动");
				beep(60);
			}
			else if (key == KEY_TIMER)		/* PE5：定时关机 10s 开/关 */
			{
				s_timer = s_timer ? 0 : 10;
				Usart_Printf("[按键] 定时关机：%s\r\n", s_timer ? "10 秒后关机" : "已取消");
				beep(60);
			}
			else if (key == KEY_GEAR)		/* PE4：亮度档位 0/30/70/100 */
			{
				uint8_t i;

				s_auto = 0;					/* 手动调光退出自动档 */

				for (i = 0; i < 4; i++)
				{
					if (s_gear[i] == s_bright)
					{
						break;
					}
				}

				i = (i >= 3) ? 0 : (uint8_t)(i + 1);

				if (!s_power)
				{
					s_power = 1;
					Usart_Printf("[按键] 已开机\r\n");
				}

				set_bright(s_gear[i]);
				Usart_Printf("[按键] 亮度 %u%%\r\n", (unsigned int)s_bright);
				beep(60);
			}
		}

		/* 3. 1 秒任务：开机计时 / 定时关机倒计时 / 周期上报 */
		if (g_flag_1s)
		{
			g_flag_1s = 0;

			if (s_power)
			{
				s_ontime++;

				if (s_timer > 0)
				{
					s_timer--;

					if (s_timer == 0)
					{
						power_off();
						Usart_Printf("[定时] 时间到，已自动关机\r\n");
						beep(120);
					}
				}

				if ((s_ontime % 5) == 0)
				{
					print_status();
				}
			}
		}

		/* 4. 自动档（呼吸灯）：每 100ms 亮度 ±1，0<->100 往返 */
		if (s_power && s_auto)
		{
			if ((Tim_GetTick() - s_last_auto) >= 100)
			{
				s_last_auto = Tim_GetTick();

				if (s_dir)
				{
					if (s_bright >= 100)
					{
						s_dir = 0;
					}
					else
					{
						set_bright((uint8_t)(s_bright + 1));
					}
				}
				else
				{
					if (s_bright <= 0)
					{
						s_dir = 1;
					}
					else
					{
						set_bright((uint8_t)(s_bright - 1));
					}
				}
			}
		}

		/* 5. 蜂鸣提示到点自动关闭 */
		if (s_beep_ms != 0 && (int32_t)(Tim_GetTick() - s_beep_ms) >= 0)
		{
			s_beep_ms = 0;
			BEEP_Off();
		}

		/* 6. 喂狗：放在主循环末尾，主循环跑飞就喂不到 */
		Eat_Dog();
	}
}
