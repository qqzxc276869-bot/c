#ifndef __CMD_H
#define __CMD_H

#include "stm32f10x.h"

/* 串口命令解析：把上位机发来的一行字符串变成对 LAMP 层的调用 */

void Cmd_Task(void);				/* 主循环里反复调用，非阻塞 */
void Cmd_Execute(char *line);		/* 直接执行一条命令（调试用） */
void Cmd_PrintHelp(void);

#endif
