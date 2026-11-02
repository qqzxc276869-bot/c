#include "USART.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)
		;//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
void Usart_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//当我们不使用它上面的普通IO口时，而是选择引脚上的其他功能时，需要选择复用功能
	//PA9 USART -TX
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeStruct);
	//PA10 USART RX
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitTypeStruct);
}

void Usart_Init(uint32_t Baud)
{
	USART_InitTypeDef USART_InitTypeStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeStruct.USART_BaudRate = Baud;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_Init(USART1,&USART_InitTypeStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART1,ENABLE);
}

void Usart_Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitTypeStruct;
	NVIC_InitTypeStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitTypeStruct);
}
//串口中断服务函数1、直收直发
void USART1_IRQHandler(void)
{
	u8 Data = 0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		
		printf("111111");
		//并没有清除标志位
		Data = USART_ReceiveData(USART1);//读取串口接收的数据，读的是USART->DR
		USART_SendData(USART1,Data);
	}
}


//判断回车并配合协议
uint8_t USART_RX_BUF[USART_REC_LEN];
uint16_t USART_RX_STA;
//void USART1_IRQHandler(void)
//{
//	uint8_t Data = 0;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
//	{
//		Data = USART_ReceiveData(USART1);//Data = USART1->DR;
//		if((USART_RX_STA & 0x8000) == 0)//判断最高为是否为0，当为0时说明可以进行接收了，如果不为0，可能数据还在处理，等数据处理完成并设置为0后再进入中断接收
//		{
//			if(USART_RX_STA & 0x4000)
//			{
//					if(Data == 0x0a)
//					{
//						USART_RX_STA |= 0x8000;
//						USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//					}
//					else
//					{
//						USART_RX_STA = 0;
//						
//					}				
//			}
//			else
//			{
//					if(Data == 0X0D)
//					{
//						USART_RX_STA |= 0x4000;
//					}
//					else//当接收到的数据不是,OXOD，说明是普通数值时，那么我们就将接收到的数据，存放到buff数组中，用于后续处理
//					{
//						USART_RX_BUF[USART_RX_STA&0X3FFF] = Data;
//						USART_RX_STA++;
//						if(USART_RX_STA > USART_REC_LEN-1)USART_RX_STA=0;
//					}
//			}
//		}
//	}
//}

//第三种写法：空闲中断

//void USART1_IRQHandler(void)
//{
//		uint8_t Data = 0;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
//	{
//		
//			Data = USART_ReceiveData(USART1);
//			USART_RX_BUF[USART_RX_STA&0X3FFF] = Data;
//			USART_RX_STA++;
//			if(USART_RX_STA > USART_REC_LEN-1)USART_RX_STA=0;
//		
//	}
//	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
//	{
//		Data = USART1->SR;
//		Data = USART1->DR;
//		USART_RX_STA |= 0x8000;
//	}
//	
//}



