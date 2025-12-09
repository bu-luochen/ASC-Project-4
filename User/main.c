#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"
#include "AD.h"
#include "Timer.h"
#include "Menu.h"
#include <string.h>
#include "W25Q64.h"
#include "Serial.h"

uint8_t KeyNum;

extern uint16_t Time_HB;

extern Menu menu[4];
extern int16_t menuIndex;
extern int16_t itemIndex;
extern int16_t mode;

uint16_t W25Q64Value[3] = {0};
float Angle[3] = {0};
uint8_t Status = 0;
static uint16_t Count_HB;
int main ()
{	
	OLED_Init();
	Key_Init();
	Timer_Init();
	Menu_Init();
	AD_Init();
	W25Q64_Init();
	Serial_Init();

	while(1)
	{
		
		if(Serial_RxFlag == 1){
			if(Serial_RxPacket[0] == 'H'){
				Status = 1;
				Count_HB = 0;
			} else {
				Serial_GetSerialPacket(Serial_RxPacket,Angle);
			}
			Serial_RxFlag = 0;
		}
		
		
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
		
		
		
		if(Time_HB >= 200){
			Time_HB = 0;
			Serial_HeartBeat();
		}
		if(Status == 1){
			OLED_ShowString(120,0,"+",OLED_8X16);
		} else {
			OLED_ShowString(120,0,"-",OLED_8X16);
		}
		
		
		
		if(menuIndex == 1){
			Menu_ShowAD();
			if(Key_Check(KEY_0,KEY_LONG)){
				W25Q64_SectorErase(0x00000000);
				W25Q64_WriteUint16(0x00000000,ADValue[0]);
				W25Q64_WriteUint16(0x00000002,ADValue[1]);
				W25Q64_WriteUint16(0x00000004,ADValue[2]);
			}
		}
		if(menuIndex == 2){
			W25Q64Value[0] = W25Q64_ReadUint16(0x00000000);
			W25Q64Value[1] = W25Q64_ReadUint16(0x00000002);
			W25Q64Value[2] = W25Q64_ReadUint16(0x00000004);
			Menu_ShowW25Q64();
		}
		if(menuIndex == 3){
			
			Menu_ShowAngle();
		}
		
		
		OLED_ShowMenu();
		
		
		
		OLED_Update();
		
		
	}
	
}

void TIM2_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		
		Key_Tick();
		HeartBeat_Tick();
		
		if(Serial_RxFlag == 0){
			Count_HB ++;
			if(Count_HB >= 500){
				Count_HB = 0;
				Status = 0;
			}
		}
		
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
}
