#include "IWDG.h"

void Iwdg_Init(uint8_t IWDG_Prescaler,uint16_t Reload)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//打开写保护
	//IWDG->KR = 0x5555;//使能，取消写保护
	IWDG_SetPrescaler(IWDG_Prescaler);
	  //IWDG->PR = 0x3;
	IWDG_SetReload(Reload);
	// IWDG->RLR = Reload;
	IWDG_ReloadCounter();
	//IWDG->KR = 0xAAAA
	IWDG_Enable();
	////IWDG->KR = 0xCCCC;
}

void Eat_Dog(void)
{
	IWDG_ReloadCounter();
}

