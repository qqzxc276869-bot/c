#include "delay.h"
void delay(u8 ms)
	{
		u8 i,j;
		for(i=ms;i>0;i--)
			for(j=110;j>0;j--);
}
