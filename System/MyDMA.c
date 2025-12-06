#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)
{
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitSturcture;
	
	MyDMA_Size = Size;
	DMA_InitSturcture.DMA_PeripheralBaseAddr = AddrA;//外设站点基地址
	DMA_InitSturcture.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//以字节传输
	DMA_InitSturcture.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//指针自增
	DMA_InitSturcture.DMA_MemoryBaseAddr = AddrB;//存储器站点基地址
	DMA_InitSturcture.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitSturcture.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitSturcture.DMA_DIR = DMA_DIR_PeripheralSRC;//外设到存储器
	DMA_InitSturcture.DMA_BufferSize = Size;
	DMA_InitSturcture.DMA_Mode = DMA_Mode_Normal;//是否自动重装(否)
	DMA_InitSturcture.DMA_M2M = DMA_M2M_Enable;//是否为存储器到存储器即软件触发
	DMA_InitSturcture.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DMA1_Channel1, &DMA_InitSturcture);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size); 
	DMA_Cmd(DMA1_Channel1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

