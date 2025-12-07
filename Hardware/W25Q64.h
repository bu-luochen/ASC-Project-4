#ifndef __W25Q64_
#define __W25Q64_

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID,uint16_t *DID);
void W25Q64_WriteEnable(void);
void W25Q64_WaitBusy(void);
void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t Count);

void W25Q64_WriteUint16(uint32_t Address, uint16_t Data);
uint16_t W25Q64_ReadUint16(uint32_t Address);
#endif

