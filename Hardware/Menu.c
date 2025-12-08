#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "OLED.h"
typedef enum {
    submenu,    
    data,      
    function,
	dec
} ItemType;

typedef struct {
    char name[20];
    ItemType type;
    float value;           
    int16_t subIndex;    
    void (*function)(void);  
} MenuItem;

typedef struct {
    char title[20];
    MenuItem item[4];  
    int16_t count;
    int16_t parIndex;     
} Menu;




Menu menu[4];

extern uint16_t ADValue[3];
extern uint16_t W25Q64Value[3];
extern float Angle[3];

void Menu_ShowAD(void)
{
	OLED_ShowNum(40,16,ADValue[0],4,OLED_8X16);
	OLED_ShowNum(40,32,ADValue[1],4,OLED_8X16);
	OLED_ShowNum(40,48,ADValue[2],4,OLED_8X16);
}

void Menu_ShowW25Q64(void)
{
	OLED_ShowNum(40,16,W25Q64Value[0],4,OLED_8X16);
	OLED_ShowNum(40,32,W25Q64Value[1],4,OLED_8X16);
	OLED_ShowNum(40,48,W25Q64Value[2],4,OLED_8X16);
}
void Menu_ShowAngle(void)
{
	OLED_ShowFloatNum(56,16,Angle[0],3,2,OLED_8X16);
	OLED_ShowFloatNum(56,32,Angle[1],3,2,OLED_8X16);
	OLED_ShowFloatNum(56,48,Angle[2],3,2,OLED_8X16);
	
}
void Menu_Init(void)
{
    strcpy(menu[0].title, "Main Menu");
    menu[0].count = 3;
    menu[0].parIndex = -1;
    strcpy(menu[0].item[0].name, "ADC");
    menu[0].item[0].type = submenu;
    menu[0].item[0].value = 0;
    menu[0].item[0].subIndex = 1;
    menu[0].item[0].function = NULL;
	strcpy(menu[0].item[1].name, "Store");
    menu[0].item[1].type = submenu;
    menu[0].item[1].value = 0;
    menu[0].item[1].subIndex = 2;
    menu[0].item[1].function = NULL;
    strcpy(menu[0].item[2].name, "IMU");
    menu[0].item[2].type = submenu;
    menu[0].item[2].value = 0;
    menu[0].item[2].subIndex = 3;
    menu[0].item[2].function = NULL;

    strcpy(menu[1].title, "ADC");
    menu[1].count = 3;
    menu[1].parIndex = 0;
    strcpy(menu[1].item[0].name,"POT");
    menu[1].item[0].type = function;
    menu[1].item[0].value = 0;
    menu[1].item[0].subIndex = -1;
    menu[1].item[0].function = NULL;
    strcpy(menu[1].item[1].name, "NTC");
    menu[1].item[1].type = function;
    menu[1].item[1].value = 0;
    menu[1].item[1].subIndex = -1;
    menu[1].item[1].function = NULL;
    strcpy(menu[1].item[2].name, "LDR");
    menu[1].item[2].type = function;
    menu[1].item[2].value = 0;
    menu[1].item[2].subIndex = -1;
    menu[1].item[2].function = NULL;
    
    strcpy(menu[2].title, "Store");
    menu[2].count = 3;
    menu[2].parIndex = 0;
    strcpy(menu[2].item[0].name,"POT");
    menu[2].item[0].type = function;
    menu[2].item[0].value = 0;
    menu[2].item[0].subIndex = -1;
    menu[2].item[0].function = NULL;
    strcpy(menu[2].item[1].name, "NTC");
    menu[2].item[1].type = function;
    menu[2].item[1].value = 0;
    menu[2].item[1].subIndex = -1;
    menu[2].item[1].function = NULL;
    strcpy(menu[2].item[2].name, "LDR");
    menu[2].item[2].type = function;
    menu[2].item[2].value = 0;
    menu[2].item[2].subIndex = -1;
    menu[2].item[2].function = NULL;
	
	strcpy(menu[3].title, "IMU");
    menu[3].count = 3;
    menu[3].parIndex = 0;
    strcpy(menu[3].item[0].name,"ROLL");
    menu[3].item[0].type = function;
    menu[3].item[0].value = 0;
    menu[3].item[0].subIndex = -1;
    menu[3].item[0].function = NULL;
    strcpy(menu[3].item[1].name, "YAW");
    menu[3].item[1].type = function;
    menu[3].item[1].value = 0;
    menu[3].item[1].subIndex = -1;
    menu[3].item[1].function = NULL;
    strcpy(menu[3].item[2].name, "PITCH");
    menu[3].item[2].type = function;
    menu[3].item[2].value = 0;
    menu[3].item[2].subIndex = -1;
    menu[3].item[2].function = NULL;
}

int16_t menuIndex = 0,itemIndex = 0,mode = 0;



void OLED_ShowMenu(void)
{

		OLED_ShowString(0,0,menu[menuIndex].title,OLED_8X16);
		for(int16_t i = 0;i < menu[menuIndex].count;i++)
		{
			if(i == itemIndex){
				OLED_ShowString(0,(i + 1) * 16,">",OLED_8X16);
			}
			OLED_ShowString(8,(i + 1) * 16,menu[menuIndex].item[i].name,OLED_8X16);

			if(menu[menuIndex].item[i].type == dec )
			{
				OLED_ShowFloatNum(88,(i + 1) * 16,menu[menuIndex].item[i].value,1,2,OLED_8X16);
			}
		}

		OLED_Update();
	
}


void up(void)
{
	itemIndex--;
	if(itemIndex < 0)
	{
	itemIndex = menu[menuIndex].count-1;
	}
}


void down(void)
{
	itemIndex++;
	if(itemIndex == menu[menuIndex].count)
	{
	itemIndex = 0;
	}
}

void back(void)
{
	if(mode == 1 && menu[menuIndex].item[itemIndex].type != function)
	{
		mode = 0;
	}else if(menu[menuIndex].parIndex != -1 && mode == 0){
		itemIndex = menuIndex - 1;
		menuIndex = menu[menuIndex].parIndex;
	}
}

void confirm(void)
{
	if(menu[menuIndex].item[itemIndex].subIndex != -1){
		menuIndex = menu[menuIndex].item[itemIndex].subIndex;
		itemIndex = 0;
	} else if(mode == 0 && menu[menuIndex].item[itemIndex].type != function)
	{
		mode = 1;
	} else if( mode == 1 && menu[menuIndex].item[itemIndex].type != function)
	{
		mode = 0;
	}
	
}

