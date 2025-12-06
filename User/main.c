#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "AD.h"
#include "Timer.h"
#include "Menu.h"
#include <string.h>

uint8_t KeyNum;

extern Menu menu[4];
extern int16_t menuIndex;
extern int16_t itemIndex;
extern int16_t mode;

int main ()
{	
	OLED_Init();
	Key_Init();
	Timer_Init();
	Menu_Init();
	AD_Init();
	
	
	
	
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
		if(menuIndex == 1){ADC_DMA_Start();Menu_ShowAD();} else {ADC_DMA_Stop();}
		
		
		
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
