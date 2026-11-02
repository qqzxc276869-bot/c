#include <REGX52.H>
#include "typedef.h"
#include "LCD1602.h"
#include "key.h"
#include "delay.h"

void main()
{
	u8 keynumber;
	LCD_Init();
	 LCD_ShowString(1,2, "Aa");
	while(1)
	{
		keynumber=key();
		if(keynumber)
		{
			LCD_ShowNum(2,1,keynumber,2);
		}
	}

}