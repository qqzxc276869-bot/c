#include "main.h"
#include "led.h"
#include "key.h"

void SystemClock_Config(void);

int main(void)
{
  uint8_t key_num = 0;

  /* 初始化 HAL，并把系统时钟配置为 72 MHz。 */
  HAL_Init();
  SystemClock_Config();

  /* 初始化 PB5 LED 输出和 PE4 按键输入。 */
  LED_Init();
  key_init();

  while (1)
  {
    /* PE4 按下一次，PB5 LED 切换一次亮灭。 */
    key_num = key_Getnum();
    if (key_num == 1)
    {
      LED1_Turn();
    }
  }
}

/* 8 MHz 外部晶振经 PLL 倍频到 72 MHz。 */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};

  /* 启动外部晶振，PLL 倍频 9 倍。 */
  oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
  {
    while (1)
    {
    }
  }

  /* 系统时钟 72 MHz，APB1 分频为 36 MHz。 */
  clkinitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2) != HAL_OK)
  {
    while (1)
    {
    }
  }
}
