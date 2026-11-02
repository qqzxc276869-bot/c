#include "gpio.h"
#include "main.h"
void gpio_init(void)
{
    /* 开启 GPIOA 外设时钟 */
   __HAL_RCC_GPIOE_CLK_ENABLE();
   GPIO_InitTypeDef GPIO_InitStruct;

   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pin = GPIO_PIN_5;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5,SET);
}