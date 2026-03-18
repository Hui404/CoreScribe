/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define cli()      __set_PRIMASK(1)   			//总中断关
#define sei()      __set_PRIMASK(0) 			//总中断开
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define XPT2046_SPI_MOSI_Pin GPIO_PIN_2
#define XPT2046_SPI_MOSI_GPIO_Port GPIOE
#define XPT2046_SPI_MISO_Pin GPIO_PIN_3
#define XPT2046_SPI_MISO_GPIO_Port GPIOE
#define XPT2046_PENIRQ_GPIO_Pin GPIO_PIN_4
#define XPT2046_PENIRQ_GPIO_GPIO_Port GPIOE
#define X_LIMIT_Pin GPIO_PIN_1
#define X_LIMIT_GPIO_Port GPIOC
#define Y_LIMIT_Pin GPIO_PIN_2
#define Y_LIMIT_GPIO_Port GPIOC
#define Z_LIMIT_Pin GPIO_PIN_3
#define Z_LIMIT_GPIO_Port GPIOC
#define X_STEP_Pin GPIO_PIN_1
#define X_STEP_GPIO_Port GPIOA
#define Y_STEP_Pin GPIO_PIN_2
#define Y_STEP_GPIO_Port GPIOA
#define Z_STEP_Pin GPIO_PIN_3
#define Z_STEP_GPIO_Port GPIOA
#define X_DIRECTION_Pin GPIO_PIN_4
#define X_DIRECTION_GPIO_Port GPIOA
#define Y_DIRECTION_Pin GPIO_PIN_5
#define Y_DIRECTION_GPIO_Port GPIOA
#define Z_DIRECTION_Pin GPIO_PIN_6
#define Z_DIRECTION_GPIO_Port GPIOA
#define STEPPERS_DISBLE_Pin GPIO_PIN_0
#define STEPPERS_DISBLE_GPIO_Port GPIOB
#define PROBE_Pin GPIO_PIN_1
#define PROBE_GPIO_Port GPIOB
#define Y_MS1_Pin GPIO_PIN_10
#define Y_MS1_GPIO_Port GPIOB
#define Y_MS2_Pin GPIO_PIN_11
#define Y_MS2_GPIO_Port GPIOB
#define X_MS1_Pin GPIO_PIN_12
#define X_MS1_GPIO_Port GPIOB
#define X_MS2_Pin GPIO_PIN_13
#define X_MS2_GPIO_Port GPIOB
#define Y_MS1B14_Pin GPIO_PIN_14
#define Y_MS1B14_GPIO_Port GPIOB
#define Y_MS2B15_Pin GPIO_PIN_15
#define Y_MS2B15_GPIO_Port GPIOB
#define ILI9341_BK_Pin GPIO_PIN_12
#define ILI9341_BK_GPIO_Port GPIOD
#define XPT2046_SPI_CS_Pin GPIO_PIN_13
#define XPT2046_SPI_CS_GPIO_Port GPIOD
#define I2C_SCL_Pin GPIO_PIN_11
#define I2C_SCL_GPIO_Port GPIOA
#define I2C_SDA_Pin GPIO_PIN_12
#define I2C_SDA_GPIO_Port GPIOA
#define CONTROL_RESET_Pin GPIO_PIN_4
#define CONTROL_RESET_GPIO_Port GPIOB
#define CONTROL_FEED_Pin GPIO_PIN_5
#define CONTROL_FEED_GPIO_Port GPIOB
#define CONTROL_CYCL_Pin GPIO_PIN_6
#define CONTROL_CYCL_GPIO_Port GPIOB
#define X_MS1B7_Pin GPIO_PIN_7
#define X_MS1B7_GPIO_Port GPIOB
#define X_MS2B9_Pin GPIO_PIN_9
#define X_MS2B9_GPIO_Port GPIOB
#define XPT2046_SPI_CLK_Pin GPIO_PIN_0
#define XPT2046_SPI_CLK_GPIO_Port GPIOE
#define ILI9341_RST_Pin GPIO_PIN_1
#define ILI9341_RST_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
