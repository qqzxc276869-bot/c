#include "led.h"
#include "stm32f1xx_hal.h"

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 开启 GPIOB 时钟，并在 PB5 切换为输出前先置低电平。 */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

  /* PB5 配置为低速推挽输出。 */
  GPIO_InitStructure.Pin = GPIO_PIN_5;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LED1_Turn(void)
{
  /* 每收到一次有效按键事件，就翻转 PB5 输出电平。 */
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}
