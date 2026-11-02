#include <REGX52.H>
#include "typedef.h"
#include "LCD1602.h"
#include "delay.h"
#include "key.h"

void main()
{
	u8 keynumber=0;
	u16 password=0,count=0;
	LCD_Init();
	LCD_ShowString(1,2, "Password");
	while(1)
	{
		keynumber=key();
		if(keynumber)
		{
			if(count<4 && keynumber<=10)	// digits: 1~9, key 10 = 0
			{
				count++;
				password*=10;
				password+=keynumber%10;		// shift left, append digit
			}
			LCD_ShowNum(2,1,password,4);

			if(keynumber==11)				// confirm key
			{
				if(password==2768)
				{
						LCD_ShowString(1,11, "right");
						password=0;				// reset and retry
					count=0;
					LCD_ShowNum(2,1,password,4);
					while(1);				// unlocked, stop here
				}
				else
				{
				LCD_ShowString(1,11, "wrong");
					password=0;				// reset and retry
					count=0;
					LCD_ShowNum(2,1,password,4);
				}
				
			}
			if(keynumber==12)
				{
					password=0;				// reset and retry
					count=0;
					LCD_ShowNum(2,1,password,4);
				}
		}
	}
}
