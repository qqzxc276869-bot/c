#include "key.h"
#include "delay.h"

u8 key()
{
	u8 keynumber=0;
	if(P3_1==0)
	{
		delay(20);
		while(P3_1==0);
		delay(20);
		keynumber=1;
	}
	if(P3_0==0)
	{
		delay(20);
		while(P3_0==0);
		delay(20);
		keynumber=2;
	}
	if(P3_2==0)
	{
		delay(20);
		while(P3_2==0);
		delay(20);
		keynumber=3;
	}
	if(P3_3==0)
	{
		delay(20);
		while(P3_3==0);
		delay(20);
		keynumber=4;
	}
	return keynumber;
}