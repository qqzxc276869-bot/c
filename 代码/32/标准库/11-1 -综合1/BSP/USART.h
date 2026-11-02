#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

/* 串口1：PA9(TX) / PA10(RX)，默认 115200-8-N-1
 * 与电脑调试助手通信：接收命令、打印状态 */

#define USART_LINE_MAX      64      /* 单条命令最大长度 */
#define USART_RX_BUF_SIZE   256     /* 接收环形缓冲区大小 */

void Usart_Gpio_Init(void);
void Usart_Nvic_Init(void);
void Usart_Init(uint32_t Baud);

/* 一次把 GPIO / NVIC / 参数都配好，推荐在 main 里调用这个 */
void Usart_All_Init(uint32_t Baud);

/* 取一行完整命令（以 \r 或 \n 结尾，或串口空闲 IDLE 断帧）
 * 返回 1 表示取到一行，0 表示还没有完整的一行。
 * 非阻塞：主循环里反复调用即可。 */
uint8_t Usart_GetLine(char *buf, uint16_t size);

void Usart_SendChar(char c);
void Usart_SendString(const char *str);

/* 轻量级格式化输出，支持 %d %u %x %X %s %c %% 以及 %02d 这类补零宽度。
 * 不依赖 C 库 stdio，因此不需要勾选 MicroLIB。 */
void Usart_Printf(const char *fmt, ...);

/* 接收溢出次数（命令太长或主循环被卡住时会增加） */
uint16_t Usart_GetOverflowCount(void);

#endif
