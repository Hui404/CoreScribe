#ifndef _24C64_H_
#define _24C64_H_

#include "main.h"
#include "MyDelay.h"
#include "stm32f4xx_hal.h"



#define AT24C64_Write_Addr		0xA0
#define AT24C64_Read_Addr		0xA1

#define AT24C64_PAGE_SIZE       32
#define AT24C64_MAX_ADDR        0x1FFF   // 8191
#define AT24C64_SIZE            8192     // 字节数

#define SDA_IN()		//I2C_SDA_GPIO_Port->MODER &= 0xFCFFFFFF
#define SDA_OUT()		//I2C_SDA_GPIO_Port->MODER &= 0xFDFFFFFF

void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);

uint8_t AT24C64_WriteOneByte(uint16_t WriteAddr, uint8_t Data);
uint8_t AT24C64_ReadOneByte(uint16_t ReadAddr, uint8_t *pData);

uint8_t AT24C64_WritePage(uint16_t WriteAddr, uint8_t *pData, uint16_t Len);
uint8_t AT24C64_ReadBytes(uint16_t ReadAddr, uint8_t *pData, uint16_t Len);
uint8_t AT24C64_WriteBytes(uint16_t WriteAddr, uint8_t *pData, uint16_t Len);

uint8_t AT24C64_Check(void);

#endif


