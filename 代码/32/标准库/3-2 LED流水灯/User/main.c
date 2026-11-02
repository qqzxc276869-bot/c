#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#define LED_A (GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_10 | GPIO_Pin_14)
#define LED_B (GPIO_Pin_1|GPIO_Pin_7)
int main(void)
{
	// ================开启A=============================
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
															//使用各个外设前必须开启时钟，否则对外设的操作无效
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = LED_A;				//GPIO引脚，赋值为所有引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函数
															//函数内部会自动根据结构体的参数配置相应寄存器
															//实现GPIOA的初始化
// ================开启B=============================
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOA的时钟
															//使用各个外设前必须开启时钟，否则对外设的操作无效
	
	/*GPIO初始化*/
				//定义结构体变量
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = LED_B;				//GPIO引脚，赋值为所有引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函数
															//函数内部会自动根据结构体的参数配置相应寄存器
															//实现GPIOA的初始化
	
	/*主循环，循环体内的代码会一直循环执行*/
	while (1)
	{


/* PA0亮 */
GPIO_SetBits(GPIOA, LED_A);
GPIO_SetBits(GPIOB, LED_B);                    // 全部置高，全部熄灭
GPIO_ResetBits(GPIOA, GPIO_Pin_0);     // PA0置低，点亮
Delay_ms(100);

/* PA1亮 */
GPIO_SetBits(GPIOA, LED_A);
GPIO_SetBits(GPIOB, LED_B); 
GPIO_ResetBits(GPIOA, GPIO_Pin_5);
Delay_ms(100);

/* PA2亮 */
GPIO_SetBits(GPIOA, LED_A);
GPIO_SetBits(GPIOB, LED_B); 
GPIO_ResetBits(GPIOA, GPIO_Pin_10);
Delay_ms(100);

/* PA3亮 */
GPIO_SetBits(GPIOA, LED_A);
GPIO_SetBits(GPIOB, LED_B); 
GPIO_ResetBits(GPIOA, GPIO_Pin_14);
Delay_ms(100);

GPIO_SetBits(GPIOB, LED_B);
GPIO_ResetBits(GPIOB, GPIO_Pin_1);
Delay_ms(100);

 GPIO_SetBits(GPIOB, LED_B);
GPIO_ResetBits(GPIOB, GPIO_Pin_7);
Delay_ms(100);
	}
}
