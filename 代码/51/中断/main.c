#include <REGX52.H>
#include "LCD1602.h"
typedef unsigned char u8;
typedef unsigned char u16;
sbit key3=P3^2;
void delay(u8 ms)
	{
		u8 i,j;
		for(i=ms;i>0;i--)
			for(j=110;j>0;j--);
}

void int0_init()//≥ı ºªØ
{  
	IT0=0;
	EX0=1;
	EA=1;
}
void int0() interrupt 0
{
	delay(10);
	if(key3==0)
	{
		P2_0=!P2_0;
		delay(200);
	}
}
void main()
{
	int0_init();
	
	while(1){}
	
}