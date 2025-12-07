#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"
#include "AD.h"
#include "Timer.h"
#include "Menu.h"
#include <string.h>
#include "W25Q64.h"

uint8_t KeyNum;

extern Menu menu[4];
extern int16_t menuIndex;
extern int16_t itemIndex;
extern int16_t mode;

uint16_t W25Q64Value[3] = {0};

int main ()
{	
	OLED_Init();
	Key_Init();
	Timer_Init();
	Menu_Init();
	AD_Init();
	W25Q64_Init();
	
	while(1)
	{
		
		if(Key_Check(KEY_4,KEY_SINGLE)){
			up();
		}
		
		if(Key_Check(KEY_2,KEY_SINGLE)){
			down();
		}
		if(Key_Check(KEY_14,KEY_SINGLE)){
			back();
			OLED_Clear();
		}
		if(Key_Check(KEY_0,KEY_SINGLE)){
			OLED_Clear();
			if (menu[menuIndex].item[itemIndex].function != NULL){
				menu[menuIndex].item[itemIndex].function();
			}
			confirm();
		}
		OLED_Clear();
		if(menuIndex == 1){
			ADC_DMA_Start();
			Menu_ShowAD();
			if(Key_Check(KEY_0,KEY_LONG)){
				W25Q64_SectorErase(0x00000000);
				W25Q64_WriteUint16(0x00000000,ADValue[0]);
				W25Q64_WriteUint16(0x00000002,ADValue[1]);
				W25Q64_WriteUint16(0x00000004,ADValue[2]);
			}
		} else {
			ADC_DMA_Stop();
		}
		if(menuIndex == 2){
			W25Q64Value[0] = W25Q64_ReadUint16(0x00000000);
			W25Q64Value[1] = W25Q64_ReadUint16(0x00000002);
			W25Q64Value[2] = W25Q64_ReadUint16(0x00000004);
			Menu_ShowW25Q64();
		}
		
		
		
		OLED_ShowMenu();
		
		
		OLED_Update();
		
		
	}
	
}

void TIM2_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){

		Key_Tick();
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
}
