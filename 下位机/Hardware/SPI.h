#ifndef __spi_H
#define __spi_H
#include "System_Config.h"

#define SPI4_NSS PEout(4)
#define SPI5_NSS PFout(6)

extern SPI_HandleTypeDef hspi1;					//SPI1���
extern SPI_HandleTypeDef SPI4_Handler;  //SPI4���
extern SPI_HandleTypeDef SPI5_Handler;  //SPI5���

void SPI4_Init(void);
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler);
uint8_t SPI4_ReadWriteByte(uint8_t TxData);

extern void _Error_Handler(char *, int);
void MX_SPI1_Init(void);

void SPI5_Init(void);
uint8_t SPI5_ReadWriteByte(uint8_t TxData);

#endif
