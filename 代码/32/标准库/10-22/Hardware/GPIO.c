#include "GPIO.h"
#include "stm32f10x.h"
// 下一步适合加入 KEY0（PE4）：让 KEY_UP 负责打开，KEY0 负责关闭。这样能把上拉输入与下拉输入放在同一个程序里练熟。
void light_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);

    /* 先预置 PB5 为高电平，LED 初始熄灭 */
    GPIO_SetBits(GPIOB, GPIO_Pin_5);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void light_on(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);  // PB5 = 0，LED 亮
}

void light_off(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_5);    // PB5 = 1，LED 灭
}
void light_turn(GPIO_TypeDef *GPIOX,uint16_t GPIO_PIN)
{
  GPIO_WriteBit(GPIOX,GPIO_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOX, GPIO_PIN)));
}

