/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* 按键编号：PA0 高电平按下；PE2、PE3、PE4 低电平按下。 */
#define KEY_PA0          0U
#define KEY_PE2          1U
#define KEY_PE3          2U
#define KEY_PE4          3U
#define KEY_COUNT        4U

#define KEY_EVENT_NONE   0U
#define KEY_EVENT_SHORT  1U
#define KEY_EVENT_LONG   2U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static volatile uint8_t key_event[KEY_COUNT] = {0U};
static uint32_t press_time[KEY_COUNT] = {0U};
static uint8_t press_flag[KEY_COUNT] = {0U};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* 每个键独立计时；松手时分类，忽略短于 50 ms 的触碰。 */
static void Key_HandleEdge(uint8_t key, uint8_t is_pressed)
{
  uint32_t hold_time;
  uint32_t now = HAL_GetTick();

  if (is_pressed != 0U)
  {
    if (press_flag[key] == 0U)
    {
      press_time[key] = now;
      press_flag[key] = 1U;
    }
  }
  else if (press_flag[key] != 0U)
  {
    hold_time = now - press_time[key];
    press_flag[key] = 0U;

    if (hold_time >= 1000U)
      key_event[key] = KEY_EVENT_LONG;
    else if (hold_time >= 50U)
      key_event[key] = KEY_EVENT_SHORT;
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint8_t key;
uint8_t event;
/* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    for (key = 0U; key < KEY_COUNT; ++key)
    {
      __disable_irq();
      event = key_event[key];
      key_event[key] = KEY_EVENT_NONE;
      __enable_irq();

      /* 沿用原功能：任意键短按翻转 PB5，长按翻转 PE5。 */
      if (event == KEY_EVENT_SHORT)
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
      else if (event == KEY_EVENT_LONG)
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* EXTI0/2/3/4 的入口由 CubeMX 写在 stm32f1xx_it.c。 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint8_t key;
  uint8_t is_pressed;

  switch (GPIO_Pin)
  {
    case GPIO_PIN_0:
      key = KEY_PA0;
      is_pressed = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET);
      break;
    case GPIO_PIN_2:
      key = KEY_PE2;
      is_pressed = (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_RESET);
      break;
    case GPIO_PIN_3:
      key = KEY_PE3;
      is_pressed = (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET);
      break;
    case GPIO_PIN_4:
      key = KEY_PE4;
      is_pressed = (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET);
      break;
    default:
      return;
  }

  Key_HandleEdge(key, is_pressed);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
