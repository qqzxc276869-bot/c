#include <reg52.h>

typedef unsigned char u8;   // 0~255
typedef unsigned int u16;   // 0~65535

sbit motor = P1^0;


void delay_us(u16 ten_us)
{
    while(ten_us--);
}


void delay_ms(u16 ms)
{
    u16 i, j;

    for(i = ms; i > 0; i--)
        for(j = 110; j > 0; j--);
}


void PWM(u8 duty)
{
    u8 i;

    for(i = 0; i < 100; i++)
    {
        if(i < duty)
        {
            motor = 1;
        }
        else
        {
            motor = 0;
        }
    }
}


void main()
{
    u16 time;

    while(1)
    {
        // 100%占空比
        for(time = 0; time < 1000; time++)
        {
            PWM(100);
        }

        // 80%占空比
        for(time = 0; time < 1000; time++)
        {
            PWM(80);
        }

        // 60%占空比
        for(time = 0; time < 1000; time++)
        {
            PWM(60);
        }

        // 40%占空比
        for(time = 0; time < 1000; time++)
        {
            PWM(40);
        }

        // 20%占空比
        for(time = 0; time < 1000; time++)
        {
            PWM(20);
        }

        // 停止
        motor = 0;

        while(1);
    }
}