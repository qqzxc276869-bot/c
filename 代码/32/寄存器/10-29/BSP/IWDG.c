#include "IWDG.h"
#include "stm32f10x.h"
void iwdg_init()
{
    IWDG->KR = 0x555;
      IWDG->PR = 4;
       IWDG->RLR = 625;
          IWDG->KR = 0xCCCC;

    /* 向KR写入0xAAAA，喂一次狗，重新加载计数值。 */
    IWDG->KR = 0xAAAA;
    
}