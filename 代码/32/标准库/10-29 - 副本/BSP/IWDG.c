#include "IWDG.h"
  /* 看门狗初始化：超时约 1 秒 */
void IWDG_Init(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable );        // 解锁 PR/RLR 写保护
    IWDG_SetPrescaler(IWDG_Prescaler_64); // 分频 64
    IWDG_SetReload(625);                // 重装载值 625
    IWDG_Enable();                      // 启动看门狗（0xCCCC 写 KR，启动后关不掉）
    IWDG_ReloadCounter();               // 先喂一次
}

 