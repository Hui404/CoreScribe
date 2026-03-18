/*
 * servo.h
 *
 *  Created on: Dec 20, 2025
 *      Author: binhui
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define SERVO_TICK_US       10U
#define SERVO_PULSE_MIN_US  1000U
#define SERVO_PULSE_MAX_US  2000U

void Servo_Init(void);
void Servo_SetPWM(uint16_t duty);
void Servo_SetPulseUs(uint16_t pulse_us);
void Servo_RequestPulseUs(uint16_t pulse_us);
void Servo_ProcessPending(void);

#endif /* INC_SERVO_H_ */
