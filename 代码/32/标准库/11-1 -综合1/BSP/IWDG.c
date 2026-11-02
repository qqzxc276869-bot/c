#include "IWDG.h"

/* ==================== 独立看门狗 ====================
 * 一旦启动就不能关闭，只能靠喂狗维持。
 * 主循环末尾调用 Eat_Dog()，若主循环卡死则约 1s 后自动复位。
 * ================================================== */

void Iwdg_Init(uint8_t IWDG_Prescaler, uint16_t Reload)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	/* 解除写保护 */

	IWDG_SetPrescaler(IWDG_Prescaler);
	IWDG_SetReload(Reload);

	IWDG_ReloadCounter();							/* 先喂一次，再启动 */
	IWDG_Enable();
}

void Iwdg_Init_1s(void)
{
	Iwdg_Init(IWDG_TIMEOUT_1S_PSC, IWDG_TIMEOUT_1S_RELOAD);
}

void Eat_Dog(void)
{
	IWDG_ReloadCounter();
}
