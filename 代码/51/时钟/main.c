#include <REGX52.H>
#include <INTRINS.h>
#include "typedef.h"
#include "LCD1602.h"
#include "delay.h"
#include "key.h"

void Time0_Init()		//1000微秒@12.000MHz
{
			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}

u16 T0count,LEDMode,h=23,m=59,s=56;
void Time_Routine() interrupt 1
{ 
//设置定时初始值
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;	
	T0count++;
	if(T0count>=1000)
		{
		T0count=0;
			s++;
			if(s>=60) 
			{
				s=0;
				m++;
			}
			if(m>=60)
			{			
				m=0;
			  h++;
			}
			if(h>=24) h=0;
			
		}
		
		
}
void main()
{
	
	
	Time0_Init();
	LCD_Init();
	while(1)
	{
	LCD_ShowNum(1,1,h,2);
	LCD_ShowNum(1,4,m,2);
	LCD_ShowNum(1,7,s,2);
	}
}

		
