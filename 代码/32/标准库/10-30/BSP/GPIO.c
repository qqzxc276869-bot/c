#include "GPIO.h"
#include "delay.h"
//void delay_us(u16 time)
//{
//	while(time--)
//	{
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();
//	}

//}
//void delay_ms(u16 time)
//{
//	while(time--)
//	{
//		delay_us(1000);		
//	}
//}


//
//LED1 PB5  LED2 PE5  BEEP PB8
//1、定义外设所用结构体
void Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	//2、使能相应外设的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);
	//3、往结构体中设置相应参数
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8;//LED1  BBEP
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//4、调用初始化函数进行初始化
	GPIO_Init(GPIOB, &GPIO_InitTypeStruct);
	
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitTypeStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
}

//key_up PA0  KEY_0  PE4 KEY_1 PE3 KEY_2 PE2
void KEY_Init(void)
{
	//1、定义外设所用结构体
	GPIO_InitTypeDef GPIO_InitTypeStruct;
		//2、使能相应外设的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
		//3、往结构体中设置相应参数
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//4、调用初始化函数进行初始化
	GPIO_Init(GPIOA,&GPIO_InitTypeStruct);
	
	//KEY_0  PE4 KEY_1 PE3 KEY_2 PE2
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIO_InitTypeStruct);
}                                
                                 

uint8_t Key_Scanf(uint8_t mode)
{
	static uint8_t stat = 1;
	uint8_t key_val = 0;
	
	if(mode)
		stat = 1;
	if(stat && ((KEY_UP==1)||(KEY_0==0)||(KEY_1==0)||(KEY_2==0)))
	{
		stat = 0;
		delay_ms(10);
		if(KEY_UP)
		key_val = 1;
		if(KEY_0==0)
		key_val = 2;
		if(KEY_1==0)
		key_val = 3;
		if(KEY_2==0)
		key_val = 4;
	}
	else if((KEY_UP==0)&&(KEY_0==1)&&(KEY_1==1)&&(KEY_2==1))
	{
		stat = 1;
	}
	return key_val;
}



void Led_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)));
}
void Beep_Turn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)));
}








