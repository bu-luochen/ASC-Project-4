#ifndef __AD_H
#define __AD_H

extern uint16_t ADValue[3];
void AD_Init(void);
void ADC_DMA_Start(void);
void ADC_DMA_Stop(void);
#endif
