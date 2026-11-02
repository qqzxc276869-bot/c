
#include "wwdg.h"


WWDG_HandleTypeDef hwwdg;
GPIO_InitTypeDef GPIO_InitStruct;

void MX_WWDG_Init(void)
{
 __HAL_RCC_WWDG_CLK_ENABLE();
 __HAL_RCC_GPIOE_CLK_ENABLE();

 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pin = GPIO_PIN_5;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(GPIOE,&GPIO_InitStruct);

 HAL_NVIC_ClearPendingIRQ(WWDG_IRQn);
 HAL_NVIC_SetPriority(WWDG_IRQn, 1, 1);
 HAL_NVIC_EnableIRQ(WWDG_IRQn);

 hwwdg.Instance = WWDG;
 hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
 hwwdg.Init.Window = 95;
 hwwdg.Init.Counter = 127;
 hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
 if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
 {
   Error_Handler();
 }


}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef* wwdgHandle)
{

  if(wwdgHandle->Instance==WWDG)
  {
    
    HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);

  }
}
