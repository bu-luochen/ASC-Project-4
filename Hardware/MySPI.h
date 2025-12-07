#ifndef __MYSPI_
#define __MYSPI_

void MySPI_W_CS(uint8_t BitValue);
void MySPI_W_SCK(uint8_t BitValue);
void MySPI_W_MOSI(uint8_t BitValue);
uint8_t MySPI_R_MISO(void);

void MySPI_Init(void);

void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);
#endif

