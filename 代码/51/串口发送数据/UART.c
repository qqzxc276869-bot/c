#include <REGX52.H>
#include "Delay.h"
void Uart1_Init(void)	//9600bps@12.000MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率

	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
// 已启用 SMOD=1
TL1 = 0xFA;
TH1 = 0xfa;
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	EA=1;
	ES=1; 
}
void UART_sendbyte(unsigned char byte)
{
	SBUF=byte;
	while(TI==0);
	TI=0;
}
void UART_routine() interrupt 4
{
	
	if(RI)
	{
		P2=~SBUF;
		UART_sendbyte(SBUF);
		RI=0;
	
	}
}