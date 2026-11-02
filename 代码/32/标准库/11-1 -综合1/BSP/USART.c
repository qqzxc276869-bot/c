#include "USART.h"
#include <stdarg.h>

/* ==========================================================
 * 串口1 驱动
 *
 * 接收：RXNE 中断把字节压进环形缓冲区；IDLE 中断表示“一帧结束”，
 *       往缓冲区补一个 \n，这样不管上位机发不发回车都能断帧。
 * 发送：直接查 TXE 标志轮询发送，配合自定义 Usart_Printf，
 *       不依赖 C 库的 printf / 半主机模式，也不用手动勾 MicroLIB。
 * ========================================================== */

static volatile uint8_t  s_rx_buf[USART_RX_BUF_SIZE];
static volatile uint16_t s_rx_head = 0;
static volatile uint16_t s_rx_tail = 0;
static volatile uint16_t s_rx_overflow = 0;

void Usart_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* PA9  USART1_TX 复用推挽 */
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitTypeStruct);

	/* PA10 USART1_RX 浮空输入 */
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitTypeStruct);
}

void Usart_Init(uint32_t Baud)
{
	USART_InitTypeDef USART_InitTypeStruct;

	USART_InitTypeStruct.USART_BaudRate = Baud;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_Init(USART1, &USART_InitTypeStruct);

	/* 接收非空中断 + 空闲中断（用于断帧） */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

void Usart_Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitTypeStruct;

	NVIC_InitTypeStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStruct);
}

void Usart_All_Init(uint32_t Baud)
{
	Usart_Gpio_Init();
	Usart_Nvic_Init();
	Usart_Init(Baud);
}

/* ---------------- 发送 ---------------- */

void Usart_SendChar(char c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}

	USART_SendData(USART1, (uint8_t)c);
}

void Usart_SendString(const char *str)
{
	while (*str != '\0')
	{
		Usart_SendChar(*str++);
	}
}

static void usart_put_uint(uint32_t value, uint8_t base, uint8_t upper, uint8_t width, char pad)
{
	char tmp[16];
	uint8_t i = 0;
	const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";

	if (value == 0)
	{
		tmp[i++] = '0';
	}

	while (value > 0)
	{
		tmp[i++] = digits[value % base];
		value /= base;
	}

	while (i < width && i < sizeof(tmp))
	{
		tmp[i++] = pad;
	}

	while (i > 0)
	{
		Usart_SendChar(tmp[--i]);
	}
}

static void usart_put_int(int32_t value, uint8_t width, char pad)
{
	if (value < 0)
	{
		Usart_SendChar('-');
		usart_put_uint((uint32_t)(-value), 10, 0, width, pad);
	}
	else
	{
		usart_put_uint((uint32_t)value, 10, 0, width, pad);
	}
}

void Usart_Printf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	while (*fmt != '\0')
	{
		uint8_t zero_pad;
		uint8_t width;

		if (*fmt != '%')
		{
			Usart_SendChar(*fmt++);
			continue;
		}

		fmt++;

		/* 解析 %0 与宽度，例如 %02d */
		zero_pad = 0;
		width = 0;
		if (*fmt == '0')
		{
			zero_pad = 1;
			fmt++;
		}
		while (*fmt >= '0' && *fmt <= '9')
		{
			width = (uint8_t)(width * 10 + (uint8_t)(*fmt - '0'));
			fmt++;
		}

		switch (*fmt)
		{
		case 'd':
		case 'i':
			usart_put_int((int32_t)va_arg(ap, int), width, zero_pad ? '0' : ' ');
			break;

		case 'u':
			usart_put_uint((uint32_t)va_arg(ap, unsigned int), 10, 0, width, zero_pad ? '0' : ' ');
			break;

		case 'x':
			usart_put_uint((uint32_t)va_arg(ap, unsigned int), 16, 0, width, zero_pad ? '0' : ' ');
			break;

		case 'X':
			usart_put_uint((uint32_t)va_arg(ap, unsigned int), 16, 1, width, zero_pad ? '0' : ' ');
			break;

		case 's':
		{
			const char *s = va_arg(ap, const char *);
			if (s == 0)
			{
				s = "(null)";
			}
			Usart_SendString(s);
			break;
		}

		case 'c':
			Usart_SendChar((char)va_arg(ap, int));
			break;

		case '%':
			Usart_SendChar('%');
			break;

		case '\0':
			va_end(ap);
			return;

		default:
			Usart_SendChar('%');
			Usart_SendChar(*fmt);
			break;
		}

		fmt++;
	}

	va_end(ap);
}

/* ---------------- 接收 ---------------- */

static void usart_ring_push(uint8_t c)
{
	uint16_t next = (uint16_t)((s_rx_head + 1) % USART_RX_BUF_SIZE);

	if (next == s_rx_tail)
	{
		s_rx_overflow++;
		return;
	}

	s_rx_buf[s_rx_head] = c;
	s_rx_head = next;
}

void USART1_IRQHandler(void)
{
	/* 收到一个字节 */
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		uint8_t data = (uint8_t)USART_ReceiveData(USART1);
		usart_ring_push(data);
	}

	/* 一帧结束（总线空闲）：补一个换行，让应用层可以断帧 */
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		volatile uint32_t tmp;

		tmp = USART1->SR;		/* 先读 SR */
		tmp = USART1->DR;		/* 再读 DR，清除 IDLE 标志 */
		(void)tmp;

		usart_ring_push('\n');
	}
}

uint16_t Usart_GetOverflowCount(void)
{
	return s_rx_overflow;
}

uint8_t Usart_GetLine(char *buf, uint16_t size)
{
	uint16_t t;
	uint16_t n = 0;
	uint8_t found = 0;

	if (buf == 0 || size < 2)
	{
		return 0;
	}

	/* 先看缓冲区里有没有行结束符，没有就一个字节都不动，避免丢半行 */
	t = s_rx_tail;
	while (t != s_rx_head)
	{
		uint8_t c = s_rx_buf[t];

		if (c == '\r' || c == '\n')
		{
			found = 1;
			break;
		}

		t = (uint16_t)((t + 1) % USART_RX_BUF_SIZE);
	}

	if (!found)
	{
		return 0;
	}

	/* 取出一行；超长的部分丢弃，但依然把它消费掉 */
	while (s_rx_tail != s_rx_head)
	{
		uint8_t c = s_rx_buf[s_rx_tail];

		s_rx_tail = (uint16_t)((s_rx_tail + 1) % USART_RX_BUF_SIZE);

		if (c == '\r' || c == '\n')
		{
			if (n == 0)
			{
				continue;		/* 跳过空行和连续的换行符 */
			}

			break;
		}

		if (n < size - 1)
		{
			buf[n++] = (char)c;
		}
	}

	buf[n] = '\0';

	return (n > 0) ? 1 : 0;
}
