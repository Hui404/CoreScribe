/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, XPT2046_SPI_MOSI_Pin|XPT2046_SPI_CLK_Pin|ILI9341_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, X_STEP_Pin|Y_STEP_Pin|Z_STEP_Pin|X_DIRECTION_Pin
                          |Y_DIRECTION_Pin|Z_DIRECTION_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, STEPPERS_DISBLE_Pin|Y_MS1_Pin|Y_MS2_Pin|X_MS1_Pin
                          |X_MS2_Pin|Y_MS1B14_Pin|Y_MS2B15_Pin|X_MS1B7_Pin
                          |X_MS2B9_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PROBE_GPIO_Port, PROBE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ILI9341_BK_Pin|XPT2046_SPI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, I2C_SCL_Pin|I2C_SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : XPT2046_SPI_MOSI_Pin XPT2046_SPI_CLK_Pin ILI9341_RST_Pin */
  GPIO_InitStruct.Pin = XPT2046_SPI_MOSI_Pin|XPT2046_SPI_CLK_Pin|ILI9341_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : XPT2046_SPI_MISO_Pin XPT2046_PENIRQ_GPIO_Pin */
  GPIO_InitStruct.Pin = XPT2046_SPI_MISO_Pin|XPT2046_PENIRQ_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : X_LIMIT_Pin Y_LIMIT_Pin Z_LIMIT_Pin */
  GPIO_InitStruct.Pin = X_LIMIT_Pin|Y_LIMIT_Pin|Z_LIMIT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : X_STEP_Pin Y_STEP_Pin Z_STEP_Pin X_DIRECTION_Pin
                           Y_DIRECTION_Pin Z_DIRECTION_Pin */
  GPIO_InitStruct.Pin = X_STEP_Pin|Y_STEP_Pin|Z_STEP_Pin|X_DIRECTION_Pin
                          |Y_DIRECTION_Pin|Z_DIRECTION_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : STEPPERS_DISBLE_Pin PROBE_Pin Y_MS1_Pin Y_MS2_Pin
                           X_MS1B7_Pin X_MS2B9_Pin */
  GPIO_InitStruct.Pin = STEPPERS_DISBLE_Pin|PROBE_Pin|Y_MS1_Pin|Y_MS2_Pin
                          |X_MS1B7_Pin|X_MS2B9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : X_MS1_Pin Y_MS1B14_Pin Y_MS2B15_Pin */
  GPIO_InitStruct.Pin = X_MS1_Pin|Y_MS1B14_Pin|Y_MS2B15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : X_MS2_Pin */
  GPIO_InitStruct.Pin = X_MS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(X_MS2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ILI9341_BK_Pin */
  GPIO_InitStruct.Pin = ILI9341_BK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(ILI9341_BK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : XPT2046_SPI_CS_Pin */
  GPIO_InitStruct.Pin = XPT2046_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(XPT2046_SPI_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : I2C_SCL_Pin I2C_SDA_Pin */
  GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CONTROL_RESET_Pin CONTROL_FEED_Pin CONTROL_CYCL_Pin */
  GPIO_InitStruct.Pin = CONTROL_RESET_Pin|CONTROL_FEED_Pin|CONTROL_CYCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
