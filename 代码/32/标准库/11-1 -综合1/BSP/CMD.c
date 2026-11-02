#include "CMD.h"
#include "LAMP.h"
#include "USART.h"
#include "TIM.h"
#include "IWDG.h"
#include "delay.h"
#include "string.h"

/* ==========================================================
 * 串口命令解析
 *
 * 协议：一行一条命令，以 \r 或 \n 结尾（没有换行也行，串口空闲
 *       中断会自动断帧）；命令名与参数之间可以用 : = 或空格。
 *       命令名不区分大小写。
 * ========================================================== */

static char *skip_space(char *s)
{
	while (*s == ' ' || *s == '\t')
	{
		s++;
	}

	return s;
}

static void trim_tail(char *s)
{
	char *p = s + strlen(s);

	while (p > s && (p[-1] == ' ' || p[-1] == '\t'))
	{
		p--;
		*p = '\0';
	}
}

static void str_upper(char *s)
{
	while (*s != '\0')
	{
		if (*s >= 'a' && *s <= 'z')
		{
			*s = (char)(*s - 'a' + 'A');
		}

		s++;
	}
}

/* 解析无符号十进制整数，必须整串都是数字 */
static uint8_t parse_uint(const char *s, uint32_t *out)
{
	uint32_t value = 0;
	uint8_t any = 0;

	if (s == 0)
	{
		return 0;
	}

	while (*s == ' ')
	{
		s++;
	}

	while (*s >= '0' && *s <= '9')
	{
		value = value * 10 + (uint32_t)(*s - '0');

		if (value > 100000UL)
		{
			return 0;
		}

		s++;
		any = 1;
	}

	while (*s == ' ' || *s == '\t')
	{
		s++;
	}

	if (*s != '\0' || !any)
	{
		return 0;
	}

	*out = value;

	return 1;
}

void Cmd_PrintHelp(void)
{
	Usart_Printf("\r\n");
	Usart_Printf("============ 台灯控制系统 命令帮助 ============\r\n");
	Usart_Printf(" STATUS / GET        查询当前状态\r\n");
	Usart_Printf(" ON / OFF            开机 / 关机\r\n");
	Usart_Printf(" TOGGLE              开关机切换\r\n");
	Usart_Printf(" SET_SPEED:n         设置亮度 0~100（也可写 BRIGHT:n）\r\n");
	Usart_Printf(" GEAR                亮度切到下一档 0/30/70/100\r\n");
	Usart_Printf(" GEAR:n              直接切到第 n 档 (0~3)\r\n");
	Usart_Printf(" MODE:AUTO|MANUAL    手动档 / 自动档\r\n");
	Usart_Printf(" TIMER:n             n 秒后自动关机，TIMER:0 取消\r\n");
	Usart_Printf(" BEEP:n              蜂鸣 n 毫秒\r\n");
	Usart_Printf(" ALARM:0|1           故障报警开关（蜂鸣器长鸣）\r\n");
	Usart_Printf(" TEST:DOG            故意死循环，验证看门狗复位\r\n");
	Usart_Printf(" TEST:RESET          软件复位\r\n");
	Usart_Printf("==============================================\r\n");
}

void Cmd_Execute(char *line)
{
	char *name;
	char *value;
	char *sep;
	uint32_t num;

	if (line == 0)
	{
		return;
	}

	str_upper(line);			/* 命令与参数统一转大写再比较 */
	name = skip_space(line);
	trim_tail(name);

	if (*name == '\0')
	{
		return;
	}

	/* 找到命令名与参数的分隔符：: = 空格 都支持 */
	sep = name;
	while (*sep != '\0' && *sep != ':' && *sep != '=' && *sep != ' ' && *sep != '\t')
	{
		sep++;
	}

	if (*sep != '\0')
	{
		*sep = '\0';
		value = skip_space(sep + 1);
		trim_tail(value);
	}
	else
	{
		value = 0;
	}

	trim_tail(name);

	if (*name == '\0')
	{
		return;
	}

	/* ---------------- 基本控制 ---------------- */

	if (strcmp(name, "HELP") == 0 || strcmp(name, "?") == 0)
	{
		Cmd_PrintHelp();
	}
	else if (strcmp(name, "STATUS") == 0 || strcmp(name, "GET") == 0)
	{
		Lamp_PrintStatus();
	}
	else if (strcmp(name, "ON") == 0)
	{
		Lamp_SetPower(LAMP_POWER_ON);
		Usart_Printf("[OK] 台灯已开启，亮度 %u%%\r\n", (unsigned int)Lamp_GetBrightness());
	}
	else if (strcmp(name, "OFF") == 0)
	{
		Lamp_SetPower(LAMP_POWER_OFF);
		Usart_Printf("[OK] 台灯已关闭\r\n");
	}
	else if (strcmp(name, "TOGGLE") == 0)
	{
		Lamp_TogglePower();
	}

	/* ---------------- 调光 ---------------- */

	else if (strcmp(name, "SET_SPEED") == 0 ||
	         strcmp(name, "SET_BRIGHT") == 0 ||
	         strcmp(name, "SPEED") == 0 ||
	         strcmp(name, "BRIGHT") == 0)
	{
		if (!parse_uint(value, &num) || num > 100)
		{
			Usart_Printf("[ERR] 用法: SET_SPEED:0~100\r\n");
		}
		else
		{
			Lamp_SetBrightness((uint8_t)num);
			Usart_Printf("[OK] 亮度已设置为 %u%%\r\n", (unsigned int)num);
		}
	}
	else if (strcmp(name, "GEAR") == 0)
	{
		if (value == 0)
		{
			Lamp_NextGear();
		}
		else if (!parse_uint(value, &num) || num >= LAMP_GEAR_NUM)
		{
			Usart_Printf("[ERR] 用法: GEAR:0~%u\r\n", (unsigned int)(LAMP_GEAR_NUM - 1));
		}
		else
		{
			Lamp_SetGear((uint8_t)num);
		}
	}

	/* ---------------- 模式 ---------------- */

	else if (strcmp(name, "MODE") == 0)
	{
		if (value == 0)
		{
			Usart_Printf("[ERR] 用法: MODE:AUTO 或 MODE:MANUAL\r\n");
		}
		else if (strcmp(value, "AUTO") == 0 || strcmp(value, "1") == 0)
		{
			Lamp_SetMode(LAMP_MODE_AUTO);
		}
		else if (strcmp(value, "MANUAL") == 0 || strcmp(value, "0") == 0)
		{
			Lamp_SetMode(LAMP_MODE_MANUAL);
		}
		else
		{
			Usart_Printf("[ERR] 未知模式: %s\r\n", value);
		}
	}
	else if (strcmp(name, "AUTO") == 0)
	{
		Lamp_SetMode(LAMP_MODE_AUTO);
	}
	else if (strcmp(name, "MANUAL") == 0)
	{
		Lamp_SetMode(LAMP_MODE_MANUAL);
	}

	/* ---------------- 定时与提示 ---------------- */

	else if (strcmp(name, "TIMER") == 0)
	{
		if (!parse_uint(value, &num) || num > 3600)
		{
			Usart_Printf("[ERR] 用法: TIMER:0~3600（秒），0 表示取消\r\n");
		}
		else
		{
			Lamp_SetOffTimer((uint16_t)num);
		}
	}
	else if (strcmp(name, "BEEP") == 0)
	{
		if (!parse_uint(value, &num) || num > 5000)
		{
			Usart_Printf("[ERR] 用法: BEEP:毫秒数\r\n");
		}
		else
		{
			Lamp_Beep((uint16_t)num);
			Usart_Printf("[OK] 蜂鸣 %u ms\r\n", (unsigned int)num);
		}
	}
	else if (strcmp(name, "ALARM") == 0)
	{
		if (!parse_uint(value, &num) || num > 1)
		{
			Usart_Printf("[ERR] 用法: ALARM:0 或 ALARM:1\r\n");
		}
		else
		{
			Lamp_SetAlarm((uint8_t)num);
		}
	}

	/* ---------------- 测试项 ---------------- */

	else if (strcmp(name, "TEST") == 0)
	{
		if (value == 0)
		{
			Usart_Printf("[ERR] 用法: TEST:DOG | TEST:RESET | TEST:ALARM\r\n");
		}
		else if (strcmp(value, "DOG") == 0)
		{
			Usart_Printf("\r\n[TEST] 进入 while(1) 死循环，主循环不再喂狗...\r\n");
			Usart_Printf("[TEST] 独立看门狗约 1s 后应复位系统\r\n");
			Usart_Printf("[TEST] 复位后请查看串口打印的复位原因是否为 IWDG\r\n");

			/* 故意死循环且不喂狗：观察系统是否被看门狗复位 */
			while (1)
			{
			}
		}
		else if (strcmp(value, "RESET") == 0)
		{
			Usart_Printf("[TEST] 软件复位 (NVIC_SystemReset)\r\n");
			delay_ms(20);
			NVIC_SystemReset();
		}
		else if (strcmp(value, "ALARM") == 0)
		{
			Lamp_SetAlarm((uint8_t)(Lamp_GetAlarm() ? 0 : 1));
		}
		else
		{
			Usart_Printf("[ERR] 未知测试项: %s\r\n", value);
		}
	}

	else
	{
		Usart_Printf("[ERR] 未知命令: %s ，输入 HELP 查看支持的命令\r\n", name);
	}
}

void Cmd_Task(void)
{
	char line[USART_LINE_MAX];

	if (Usart_GetLine(line, sizeof(line)))
	{
		Cmd_Execute(line);
	}
}
