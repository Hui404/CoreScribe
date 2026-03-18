/*
 * XPT2046_touch.h
 *
 *  Created on: Feb 27, 2026
 *      Author: xuuuu404
 */

#ifndef INC_XPT2046_TOUCH_H_
#define INC_XPT2046_TOUCH_H_

#include "main.h"
#include "stm32f4xx_hal.h"

//检测触摸屏按下和松开
#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//触摸消抖阈值
#define DURIATION_TIME				2

typedef	struct          //液晶坐标结构体
{
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y;

	/*用于记录连续触摸时(长按)的上一次触摸位置*/
	 int16_t pre_x;
   int16_t pre_y;

} strType_XPT2046_Coordinate;

typedef struct         //校准系数结构体（最终使用）
{
	 float dX_X,
					dX_Y,
					dX,
					dY_X,
					dY_Y,
					dY;

} strType_XPT2046_TouchPara;

typedef enum
{
	XPT2046_STATE_RELEASE  = 0,	//触摸释放
	XPT2046_STATE_WAITING,			//触摸按下
	XPT2046_STATE_PRESSED,			//触摸按下
}enumTouchState	;

#define	            XPT2046_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字
#define	            XPT2046_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字


/******************************* XPT2046 触摸屏触摸信号指示引脚定义(不使用中断) ***************************/
#define             XPT2046_PENIRQ_GPIO_CLK                        RCC_APB2Periph_GPIOE
#define             XPT2046_PENIRQ_GPIO_PORT                       GPIOE
#define             XPT2046_PENIRQ_GPIO_PIN                        GPIO_PIN_4

//触屏信号有效电平
#define             XPT2046_PENIRQ_ActiveLevel                     0
#define             XPT2046_PENIRQ_Read()                          HAL_GPIO_ReadPin ( XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN )



/******************************* XPT2046 触摸屏模拟SPI引脚定义 ***************************/
#define             XPT2046_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOD

#define             XPT2046_SPI_CS_PIN		                     GPIO_PIN_13
#define             XPT2046_SPI_CS_PORT		                     GPIOD

#define	            XPT2046_SPI_CLK_PIN	                         GPIO_PIN_0
#define             XPT2046_SPI_CLK_PORT	                     GPIOE

#define	            XPT2046_SPI_MOSI_PIN	                     GPIO_PIN_2
#define	            XPT2046_SPI_MOSI_PORT	                     GPIOE

#define	            XPT2046_SPI_MISO_PIN	                     GPIO_PIN_3
#define	            XPT2046_SPI_MISO_PORT	                     GPIOE


#define             XPT2046_CS_ENABLE()                          HAL_GPIO_WritePin ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, 1)
#define             XPT2046_CS_DISABLE()                         HAL_GPIO_WritePin ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, 0)

#define             XPT2046_CLK_HIGH()                           HAL_GPIO_WritePin ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, 1)
#define             XPT2046_CLK_LOW()                            HAL_GPIO_WritePin ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, 0)

#define             XPT2046_MOSI_1()                             HAL_GPIO_WritePin ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, 1)
#define             XPT2046_MOSI_0()                             HAL_GPIO_WritePin ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, 0)

#define             XPT2046_MISO()                               HAL_GPIO_ReadPin( XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN )


void XPT2046_Init(void);
void XPT2046_WriteCMD(uint8_t ucCmd);
uint16_t XPT2046_ReadCMD (void);
uint8_t XPT2046_TouchDetect(void);
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara );


uint16_t XPT2046_GetPoint_X(void);
uint16_t XPT2046_GetPoint_Y(void);



#endif /* INC_XPT2046_TOUCH_H_ */
