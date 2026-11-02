#include "reg52.h"

typedef unsigned int u16;
typedef unsigned char u8;

sbit int_a=P1^0;
sbit int_b=P1^1;
sbit int_c=P1^2;
sbit int_d=P1^3;
sbit key1=P3^1;
sbit key2=P3^0;
sbit key3=P3^2;
sbit key4=P3^3;
#define key1_press 1
#define key2_press 2
#define key3_press 3
#define key4_press 4
#define  maxspeed 5
#define  minspeed  15
void delay(u8 ms)
{
	u8 i,j;
	for(i=ms;i>0;i--){
	for(j=110;j>0;j--);
	}
}
void step_motor(u8 step,u8 dir)
{ 
	u8 temp=step;
	if(dir==0)
		{
		temp=7-step;
	}
	switch(temp)
	{
		case 0:int_a=1;int_b=0;int_c=0;int_d=0;break;
		case 1: int_a=1;int_b=1;int_c=0;int_d=0;break;
		case 2: int_a=0;int_b=1;int_c=0;int_d=0;break;
		case 3: int_a=0;int_b=1;int_c=1;int_d=0;break;
		case 4: int_a=0;int_b=0;int_c=1;int_d=0;break;
		case 5: int_a=0;int_b=0;int_c=1;int_d=1;break;
		case 6: int_a=0;int_b=0;int_c=0;int_d=1;break;
		case 7: int_a=1;int_b=0;int_c=0;int_d=1;break;
		
	}
	
}
 u8 key_scan(u8 mode)
{
 static  u8 key=1;
	if(mode)key=1;
	if(key==1&&(key1==0||key2==0||key3==0||key4==0))
	{
		delay(20);
   	key=0;
		if(key1==0) return key1_press;
		if(key2==0) return key2_press;
		if(key3==0) return key3_press;
		if(key4==0) return key4_press;
		
	}
	 else if(key1==1 && key2==1 && key3==1 && key4==1)
    {
        key=1;
    }

    return 0;
}
void main()
{
	u8 key=0;
	u8 dir=0;
	u8 speed=maxspeed;
	u8 step=0;
	u8 motor_run=1;
	
	while(1)
	{
		key=key_scan(0);
	if(key==key1_press) 
	{dir=!dir;
	}
		else if(key==key2_press) 
		{
			if(speed>maxspeed)	speed--;
		}
			else	if(key==key3_press)
			{			
				if(speed<minspeed)	speed++;
			}
		else if(key==key4_press)
		{		
			motor_run=!motor_run;
		}
	  if(motor_run){
		step_motor(step++,dir);
	  if(step==8) step=0;
	}
	delay(speed);
	}
}




