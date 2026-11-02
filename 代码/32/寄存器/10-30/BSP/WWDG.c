#include "WWDG.h"
#include "stm32f10x.h"
void Wwdg_init()
{
    RCC->APB1ENR |= (1u << 11);
    WWDG->CFR |= (0x11 << 7) | (0x5f);



    WWDG->CR = (1U << 7) | 0x7F;

}