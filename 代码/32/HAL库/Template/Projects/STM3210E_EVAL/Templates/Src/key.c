/* PE4 按键模块：按键接 GND，松开为高电平，按下为低电平。 */
/* 按下和松开各消抖 20 ms，一次完整按键只返回一次按键值。 */
#include "key.h"
#include "stm32f1xx_hal.h"

/* 初始化 PE4 上拉输入。 */
void key_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* PE4 内部上拉：松开为高电平，按键接 GND 时按下为低电平。 */
  GPIO_InitStructure.Pin = GPIO_PIN_4;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/* 按下一次并松开后返回 1，未按下时返回 0。 */
uint8_t key_Getnum(void)
{
  uint8_t keynum = 0;

  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
  {
    HAL_Delay(20);  /* 按下消抖。 */
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
    {
      /* 等待松开，避免长按时重复触发。 */
      while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
      {
      }
      HAL_Delay(20);  /* 松开消抖。 */
      keynum = 1;
    }
  }

  return keynum;
}
