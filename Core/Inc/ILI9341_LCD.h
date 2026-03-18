/*
 * ILI9341_LCD.h
 *
 *  Created on: Feb 27, 2026
 *      Author: xuuuu404
 */

#ifndef INC_ILI9341_LCD_H_
#define INC_ILI9341_LCD_H_

#include <stdio.h>

#include "main.h"
#include "stm32f4xx_hal.h"
#include "MyDelay.h"

#define	ILI9341_CMD_ADDR	( uint16_t*)(0x60000000)
#define	ILI9341_DATA_ADDR	( uint16_t*)(0x60020000)



#define      DEBUG_DELAY()		MyDelay_ms(1);

#define 	 FSMC_BANK_NORSRAM_LCD	FSMC_Bank1_NORSRAM1

#define		RGB888_To_RGB565(R, G, B)		(uint16_t)(((R & 0x1F) << 11 ) | ((G & 0x3F) << 5 ) | (B & 0x1F ))


/*----LCD显示区域及方向----*/
#define		ILI9341_DispWindow_X_Star		0		//起始点的X坐标
#define		ILI9341_DispWindow_X_Star		0		//起始点的Y坐标

#define		ILI9341_LESS_PIXEL				240		//液晶屏短边方向的像素宽度
#define		ILI9341_MORE_PIXEL				320		//液晶屏长边方向的像素宽度

/*----LCD控制信号----*/

//CS引脚
#define			ILI9341_CS_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_CS_PORT		GPIOD
#define			ILI9341_CS_PIN		GPIO_Pin_7

//复位引脚
#define			ILI9341_RST_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_RST_PORT	GPIOE
#define			ILI9341_RST_PIN		GPIO_Pin_1

//背光引脚
#define			ILI9341_BK_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_BK_PORT		GPIOD
//#define			ILI9341_BK_PIN		GPIO_Pin_12

//读引脚
#define			ILI9341_RD_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_RD_PORT		GPIOD
#define			ILI9341_RD_PIN		GPIO_Pin_4

//写引脚
#define			ILI9341_WR_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_WR_PORT		GPIOD
#define			ILI9341_WR_PIN		GPIO_Pin_5

//数据命令选择引脚
#define			ILI9341_DC_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_DC_PORT		GPIOD
#define			ILI9341_DC_PIN		GPIO_Pin_11

/*----LCD数据线----*/

//FSMC_D0
#define			ILI9341_D0_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_D0_PORT		GPIOD
#define			ILI9341_D0_PIN		GPIO_Pin_14

//FSMC_D1
#define			ILI9341_D1_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_D1_PORT		GPIOD
#define			ILI9341_D1_PIN		GPIO_Pin_15

//FSMC_D2
#define			ILI9341_D2_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_D2_PORT		GPIOD
#define			ILI9341_D2_PIN		GPIO_Pin_0

//FSMC_D3
#define			ILI9341_D3_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_D3_PORT		GPIOD
#define			ILI9341_D3_PIN		GPIO_Pin_1

//FSMC_D4
#define			ILI9341_D4_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D4_PORT		GPIOE
#define			ILI9341_D4_PIN		GPIO_Pin_7

//FSMC_D5
#define			ILI9341_D5_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D5_PORT		GPIOE
#define			ILI9341_D5_PIN		GPIO_Pin_8

//FSMC_D6
#define			ILI9341_D6_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D6_PORT		GPIOE
#define			ILI9341_D6_PIN		GPIO_Pin_9

//FSMC_D7
#define			ILI9341_D7_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D7_PORT		GPIOE
#define			ILI9341_D7_PIN		GPIO_Pin_10

//FSMC_D8
#define			ILI9341_D8_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D8_PORT		GPIOE
#define			ILI9341_D8_PIN		GPIO_Pin_11

//FSMC_D9
#define			ILI9341_D9_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D9_PORT		GPIOE
#define			ILI9341_D9_PIN		GPIO_Pin_12

//FSMC_D10
#define			ILI9341_D10_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D10_PORT	GPIOE
#define			ILI9341_D10_PIN		GPIO_Pin_13

//FSMC_D11
#define			ILI9341_D11_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D11_PORT	GPIOE
#define			ILI9341_D11_PIN		GPIO_Pin_14

//FSMC_D12
#define			ILI9341_D12_CLK		RCC_AHB2Periph_GPIOE
#define			ILI9341_D12_PORT	GPIOE
#define			ILI9341_D12_PIN		GPIO_Pin_15

//FSMC_D13
#define			ILI9341_D13_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_D13_PORT	GPIOD
#define			ILI9341_D13_PIN		GPIO_Pin_8

//FSMC_D14
#define			ILI9341_D14_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_D14_PORT	GPIOD
#define			ILI9341_D14_PIN		GPIO_Pin_9

//FSMC_D15
#define			ILI9341_D15_CLK		RCC_AHB2Periph_GPIOD
#define			ILI9341_D15_PORT	GPIOD
#define			ILI9341_D15_PIN		GPIO_Pin_10


/*----ILI9341命令----*/
#define			CMD_SetCoordinateX		0x2A		//设置X坐标
#define			CMD_SetCoordinateY		0x2B		//设置Y坐标
#define			CMD_SetPixel			0x2C		//填充像素







/*----函数----*/

void ILI9341_LCD_Init(void);
void ILI9341_Draw_Rec(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight, uint32_t Color);
void ILI9341_Clear(void);

void ILI9341_Draw_Rec_LVGL(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color_array);




#endif /* INC_ILI9341_LCD_H_ */
