/*
 * ILI9341_LCD.c
 *
 *  Created on: Feb 27, 2026
 *      Author: xuuuu404
 */
#include "ILI9341_LCD.h"
#include "fsmc.h"


//根据液晶扫描方向而改变XY像素宽度
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;


//液晶屏扫描模式，本变量主要用于方便选择触摸屏的计算参数
//参数可选值为0-7
//调用ILI9341_GramScan函数设置方向时会自动更改
//LCD刚初始化完成时会使用本默认值
uint8_t LCD_SCAN_MODE=5;			//5：横屏  6：竖屏

static __inline void ILI9341_Write_Cmd( uint16_t usCmd)
{
	*ILI9341_CMD_ADDR = usCmd;

}


static __inline void ILI9341_Write_Data( uint16_t usData)
{
	*ILI9341_DATA_ADDR = usData;

}


__inline uint16_t ILI9341_Read_Data(void)
{
	return *ILI9341_DATA_ADDR;

}

//简单延时
static void ILI9341_Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

//LCD复位
void ILI9341_LCD_RST(void)
{
	HAL_GPIO_WritePin(ILI9341_RST_GPIO_Port, ILI9341_RST_Pin, 0);
	ILI9341_Delay(0xAFF);

	HAL_GPIO_WritePin(ILI9341_RST_GPIO_Port, ILI9341_RST_Pin, 1);

	ILI9341_Delay(0xAFF);
}

//背光
void ILI9341_LCD_BK(uint8_t flag)
{
	if(flag)
	{
		HAL_GPIO_WritePin(ILI9341_BK_GPIO_Port, ILI9341_BK_Pin, 0);
	}
	else
	{
		HAL_GPIO_WritePin(ILI9341_BK_GPIO_Port, ILI9341_BK_Pin, 1);
	}

}

//ILI9341寄存器初始化
void ILI9341_Reg_Init(void)
{
	    /*  Power control B (CFh)  */
    DEBUG_DELAY  ();
    ILI9341_Write_Cmd ( 0xCF  );
    ILI9341_Write_Data ( 0x00  );
    ILI9341_Write_Data ( 0x81  );
    ILI9341_Write_Data ( 0x30  );

    /*  Power on sequence control (EDh) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xED );
    ILI9341_Write_Data ( 0x64 );
    ILI9341_Write_Data ( 0x03 );
    ILI9341_Write_Data ( 0x12 );
    ILI9341_Write_Data ( 0x81 );

    /*  Driver timing control A (E8h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xE8 );
    ILI9341_Write_Data ( 0x85 );
    ILI9341_Write_Data ( 0x10 );
    ILI9341_Write_Data ( 0x78 );

    /*  Power control A (CBh) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xCB );
    ILI9341_Write_Data ( 0x39 );
    ILI9341_Write_Data ( 0x2C );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x34 );
    //ILI9341_Write_Data ( 0x02 );
    ILI9341_Write_Data ( 0x06 ); //原来是0x02改为0x06可防止液晶显示白屏时有条纹的情况

    /* Pump ratio control (F7h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xF7 );
    ILI9341_Write_Data ( 0x20 );

    /* Driver timing control B */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xEA );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );

    /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB1 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x1B );

    /*  Display Function Control (B6h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB6 );
    ILI9341_Write_Data ( 0x0A );
    ILI9341_Write_Data ( 0xA2 );

    /* Power Control 1 (C0h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC0 );
    ILI9341_Write_Data ( 0x35 );

    /* Power Control 2 (C1h) */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC1 );
    ILI9341_Write_Data ( 0x11 );

    /* VCOM Control 1 (C5h) */
    ILI9341_Write_Cmd ( 0xC5 );
    ILI9341_Write_Data ( 0x45 );
    ILI9341_Write_Data ( 0x45 );

    /*  VCOM Control 2 (C7h)  */
    ILI9341_Write_Cmd ( 0xC7 );
    ILI9341_Write_Data ( 0xA2 );

    /* Enable 3G (F2h) */
    ILI9341_Write_Cmd ( 0xF2 );
    ILI9341_Write_Data ( 0x00 );

    /* Gamma Set (26h) */
    ILI9341_Write_Cmd ( 0x26 );
    ILI9341_Write_Data ( 0x01 );
    DEBUG_DELAY ();

    /* Positive Gamma Correction */
    ILI9341_Write_Cmd ( 0xE0 ); //Set Gamma
    ILI9341_Write_Data ( 0x0F );
    ILI9341_Write_Data ( 0x26 );
    ILI9341_Write_Data ( 0x24 );
    ILI9341_Write_Data ( 0x0B );
    ILI9341_Write_Data ( 0x0E );
    ILI9341_Write_Data ( 0x09 );
    ILI9341_Write_Data ( 0x54 );
    ILI9341_Write_Data ( 0xA8 );
    ILI9341_Write_Data ( 0x46 );
    ILI9341_Write_Data ( 0x0C );
    ILI9341_Write_Data ( 0x17 );
    ILI9341_Write_Data ( 0x09 );
    ILI9341_Write_Data ( 0x0F );
    ILI9341_Write_Data ( 0x07 );
    ILI9341_Write_Data ( 0x00 );

    /* Negative Gamma Correction (E1h) */
    ILI9341_Write_Cmd ( 0XE1 ); //Set Gamma
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x19 );
    ILI9341_Write_Data ( 0x1B );
    ILI9341_Write_Data ( 0x04 );
    ILI9341_Write_Data ( 0x10 );
    ILI9341_Write_Data ( 0x07 );
    ILI9341_Write_Data ( 0x2A );
    ILI9341_Write_Data ( 0x47 );
    ILI9341_Write_Data ( 0x39 );
    ILI9341_Write_Data ( 0x03 );
    ILI9341_Write_Data ( 0x06 );
    ILI9341_Write_Data ( 0x06 );
    ILI9341_Write_Data ( 0x30 );
    ILI9341_Write_Data ( 0x38 );
    ILI9341_Write_Data ( 0x0F );

    /* memory access control set */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0x36 );
    ILI9341_Write_Data ( 0xC8 );    /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
    DEBUG_DELAY ();

    /* column address control set */
    ILI9341_Write_Cmd ( CMD_SetCoordinateX );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0xEF );

    /* page address control set */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( CMD_SetCoordinateY );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x01 );
    ILI9341_Write_Data ( 0x3F );

    /*  Pixel Format Set (3Ah)  */
    DEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0x3a );
    ILI9341_Write_Data ( 0x55 );

    /* Sleep Out (11h)  */
    ILI9341_Write_Cmd ( 0x11 );
    ILI9341_Delay ( 0xAFFf<<2 );
    DEBUG_DELAY ();

    /* Display ON (29h) */
    ILI9341_Write_Cmd ( 0x29 );








}


/**
 * @brief  设置ILI9341的GRAM的扫描方向
 * @param  ucOption ：选择GRAM的扫描方向
 *     @arg 0-7 :参数可选值为0-7这八个方向
 *
 *	！！！其中0、3、5、6 模式适合从左至右显示文字，
 *				不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果
 *
 *	其中0、2、4、6 模式的X方向像素为240，Y方向像素为320
 *	其中1、3、5、7 模式下X方向像素为320，Y方向像素为240
 *
 *	其中 6 模式为大部分液晶例程的默认显示方向
 *	其中 3 模式为摄像头例程使用的方向
 *	其中 0 模式为BMP图片显示例程使用的方向
 *
 * @retval 无
 * @note  坐标图例：A表示向上，V表示向下，<表示向左，>表示向右
					X表示X轴，Y表示Y轴

											 LCDID_ILI9341
------------------------------------------------------------
模式0：				.		模式1：		.	模式2：			.	模式3：
					A		.					A		.		A					.		A
					|		.					|		.		|					.		|
					Y		.					X		.		Y					.		X
					0		.					1		.		2					.		3
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->
------------------------------------------------------------
模式4：				.	模式5：			.	模式6：			.	模式7：
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->
					4		.					5		.		6					.		7
					Y		.					X		.		Y					.		X
					|		.					|		.		|					.		|
					V		.					V		.		V					.		V
---------------------------------------------------------
											 LCD屏示例
								|-----------------|
								|			野火Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								屏幕正面（宽240，高320）




								      LCDID_ST7789V
------------------------------------------------------------
模式0：				.		模式1：		.	模式2：			.	模式3：
	o 0X--->  	.		o 1Y--->  .	<--- X2 o		.	<--- Y3 o
	0						.		1					.					2		.					3
	Y						.		X					.					Y		.					X
	|						.		|					.					|		.					|
	V								V					.					V		.					V
------------------------------------------------------------
模式4：				.	模式5：			.	模式6：		.	模式7：
	A						.		A					.					A	.					A
	|						.		|					.					|	.					|
	Y						.		X					.					Y	.					X
	4						.		5					.					6	.					7
	o 4X--->  	.		o 5Y--->  .	<--- X6 o	.	<--- Y7 o
---------------------------------------------------------
											 LCD屏示例
								|-----------------|
								|			野火Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								屏幕正面（宽240，高320）

 *******************************************************/
void ILI9341_GramScan ( uint8_t ucOption )
{
	//参数检查，只可输入0-7
	if(ucOption >7 )
		return;

	//根据模式更新LCD_SCAN_MODE的值，主要用于触摸屏选择计算参数
	LCD_SCAN_MODE = ucOption;

	//根据模式更新XY方向的像素宽度
	if(ucOption%2 == 0)
	{
		//0 2 4 6模式下X方向像素宽度为240，Y方向为320
		LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		LCD_Y_LENGTH =	ILI9341_MORE_PIXEL;
	}
	else
	{
		//1 3 5 7模式下X方向像素宽度为320，Y方向为240
		LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		LCD_Y_LENGTH =	ILI9341_LESS_PIXEL;
	}

	//0x36命令参数的高3位可用于设置GRAM扫描方向
	ILI9341_Write_Cmd ( 0x36 );

    ILI9341_Write_Data ( 0x08 |(ucOption<<5));//根据ucOption的值设置LCD参数，共0-7种模式


	ILI9341_Write_Cmd ( CMD_SetCoordinateX );
	ILI9341_Write_Data ( 0x00 );		/* x 起始坐标高8位 */
	ILI9341_Write_Data ( 0x00 );		/* x 起始坐标低8位 */
	ILI9341_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x 结束坐标高8位 */
	ILI9341_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x 结束坐标低8位 */

	ILI9341_Write_Cmd ( CMD_SetCoordinateY );
	ILI9341_Write_Data ( 0x00 );		/* y 起始坐标高8位 */
	ILI9341_Write_Data ( 0x00 );		/* y 起始坐标低8位 */
	ILI9341_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y 结束坐标高8位 */
	ILI9341_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );				/* y 结束坐标低8位 */

	/* write gram start */
	ILI9341_Write_Cmd ( CMD_SetPixel );
}



void ILI9341_LCD_Init(void)
{
//	ILI9341_GPIO_Init();
//	ILI9341_FSMC_Init();

	//MX_FSMC_Init();
	//HAL_FSMC_MspInit();

	ILI9341_LCD_BK(1);
	ILI9341_LCD_RST();

	ILI9341_Reg_Init();

	ILI9341_GramScan(LCD_SCAN_MODE); //设置屏幕方向


}

//打开一个窗口
void ILI9341_Open_Window(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 				 /* 设置X坐标 */
	ILI9341_Write_Data ( usX >> 8  );	 /* 先高8位，然后低8位 */
	ILI9341_Write_Data ( usX & 0xff  );	 /* 设置起始点和结束点*/
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 			     /* 设置Y坐标*/
	ILI9341_Write_Data ( usY >> 8  );
	ILI9341_Write_Data ( usY & 0xff  );
	ILI9341_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
	ILI9341_Write_Data ( ( usY + usHeight - 1) & 0xff );

}



//绘制一个矩形，并填充颜色
void ILI9341_Draw_Rec(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight, uint32_t Color)
{
	uint32_t i;
	uint8_t  R8, G8, B8;
	uint8_t  R5, G6, B5;
	uint16_t RGB;

	R8 = (Color >> 16) & 0xFF;
	G8 = (Color >> 8)  & 0xFF;
	B8 =  Color & 0xFF;

	R5 = (R8 >> 3) & 0x1F;
	G6 = (G8 >> 3) & 0x3F;
	B5 = (B8 >> 3) & 0x1F;

	RGB = ((R5 << 11) | (G6 << 5) | B5);

	ILI9341_Open_Window(usX, usY ,usWidth ,usHeight);

	ILI9341_Write_Cmd(CMD_SetPixel);

	//for(i = 0; i<(usWidth*usHeight); i++) ILI9341_Write_Data(RGB888_To_RGB565(204, 0, 0));
	for(i = 0; i<(usWidth*usHeight); i++) ILI9341_Write_Data(RGB);
	//for(i = 0; i<(usWidth*usHeight); i++) ILI9341_Write_Data( R>>3 | G | B>>3 );
}

//清除
void ILI9341_Clear(void)
{
	uint32_t i;
	ILI9341_Open_Window(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
	ILI9341_Write_Cmd(CMD_SetPixel);

	for(i = 0; i<(320*280); i++) ILI9341_Write_Data(0xFFFF);

}


/**
  * @brief  为LVGL设计的绘制矩形函数（接收颜色数组）
  * @param  x1, y1: 矩形左上角坐标
  * @param  x2, y2: 矩形右下角坐标
  * @param  color_array: 颜色数组指针（RGB565格式）
  * @retval None
  */
void ILI9341_Draw_Rec_LVGL(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t* color_array)
{
    uint32_t i;
    uint16_t width = x2 - x1 + 1;
    uint16_t height = y2 - y1 + 1;
    uint32_t pixel_count = (uint32_t)width * height;

    // 设置窗口
    ILI9341_Open_Window(x1, y1, width, height);

    // 发送写像素命令
    ILI9341_Write_Cmd(CMD_SetPixel);

    // 批量写入颜色数据
    for(i = 0; i < pixel_count; i++) {
        ILI9341_Write_Data(color_array[i]);
    }
}








