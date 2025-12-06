#include "stm32f10x.h"                  // Device header

uint16_t ADValue[3];

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//6分频
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ADC专用
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_55Cycles5);
	
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发源选择(这里none等于软件触发)
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ENABLE连续模式，DISABLE非连续
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ENABLE扫描模式，DISABLE非扫描
	ADC_InitStructure.ADC_NbrOfChannel = 3;//通道数目
	ADC_Init(ADC1,&ADC_InitStructure);
	

	
	
	DMA_InitTypeDef DMA_InitSturcture;
	
	DMA_InitSturcture.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//外设站点基地址
	DMA_InitSturcture.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitSturcture.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//指针不自增
	DMA_InitSturcture.DMA_MemoryBaseAddr = (uint32_t)ADValue;//存储器站点基地址
	DMA_InitSturcture.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitSturcture.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitSturcture.DMA_DIR = DMA_DIR_PeripheralSRC;//外设到存储器
	DMA_InitSturcture.DMA_BufferSize = 3;
	DMA_InitSturcture.DMA_Mode = DMA_Mode_Circular;//是否自动重装(否)
	DMA_InitSturcture.DMA_M2M = DMA_M2M_Disable;//是否为存储器到存储器即软件触发
	DMA_InitSturcture.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DMA1_Channel1, &DMA_InitSturcture);
	
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);//复位
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//复位状态
	ADC_StartCalibration(ADC1);//开始校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);//校准状态
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void ADC_DMA_Start(void)
{
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
}

void ADC_DMA_Stop(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	ADC_DMACmd(ADC1,DISABLE);
	ADC_Cmd(ADC1,DISABLE);
}

