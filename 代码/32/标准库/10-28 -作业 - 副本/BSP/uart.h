/*
 * uart.h 提供串口模块的函数声明。
 * 其他 .c 文件包含本头文件后，就可以调用 uart_init()。
 * 协议解析和命令执行属于 uart.c 的内部实现，不需要在这里暴露。
 */
#ifndef UART_H
#define UART_H
/*
 * 上面的头文件保护用于避免同一编译单元重复包含本文件。
 * 第一次包含时定义 UART_H；再次包含时，预处理器跳过保护范围内的内容。
 */

/* 提供 STM32 外设类型、uint8_t/uint32_t 和工程启用的标准库接口。 */
#include "stm32f10x.h"

/* 提供 FILE 类型，使下面的 fputc() 声明能够被编译器识别。 */
#include <stdio.h>

/*
 * 功能：配置 USART1 的 GPIO、通信参数和接收中断。
 * 参数 baud：波特率，例如 uart_init(9600) 或 uart_init(115200)。
 * 返回值：无。
 *
 * 调用前，系统时钟应已配置，并在 main() 中设置好 NVIC 优先级分组。
 * LED 和蜂鸣器的输出初始化由 Gpio_Init() 完成，不在本函数内完成。
 * 正常运行时只需在上电初始化阶段调用一次。
 */
void uart_init(uint32_t baud);

/*
 * 功能：提供把一个字符发送到 USART1 的 fputc 实现，供 printf 重定向使用。
 * 参数 ch：要发送的字符；参数 f：标准输出接口要求的文件参数，本实现不使用。
 * 返回值：返回传入的字符 ch。
 *
 * 实际 printf 重定向还依赖工程现有的 C 库链接配置。
 * 本函数需要等待发送寄存器空闲，适合在前台调试输出，不要放入接收中断。
 */
int fputc(int ch, FILE *f);

#endif
