/*
 * servo.c
 *
 *  Created on: Dec 20, 2025
 *      Author: binhui
 */
#include "servo.h"
#include "tim.h"

static volatile uint16_t servo_pending_pulse_us;	//输入的PWM值
static volatile uint8_t servo_update_pending;		//更新标志位

//确定PWM（us）的最大值和最小值，避免超出
static uint16_t Servo_ClampPulseUs(uint16_t pulse_us)
{
	if (pulse_us < SERVO_PULSE_MIN_US) {
		return SERVO_PULSE_MIN_US;
	}
	if (pulse_us > SERVO_PULSE_MAX_US) {
		return SERVO_PULSE_MAX_US;
	}
	return pulse_us;
}

static uint16_t Servo_PulseUsToCompare(uint16_t pulse_us)
{
	//当前定时器4的频率为 84000000/840 = 10 0000
	//故1s计100000个数，即10us计1个数

	return Servo_ClampPulseUs(pulse_us) / SERVO_TICK_US;
}

void Servo_Init(void)
{
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	servo_pending_pulse_us = 0U;
	servo_update_pending = 0U;
}

void Servo_SetPWM(uint16_t duty)
{
	//确定PWM（CCR值）的最大值和最小值，避免超出
	const uint16_t min_compare = SERVO_PULSE_MIN_US / SERVO_TICK_US;
	const uint16_t max_compare = SERVO_PULSE_MAX_US / SERVO_TICK_US;

	if (duty < min_compare) {
		duty = min_compare;
	} else if (duty > max_compare) {
		duty = max_compare;
	}

	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, duty);

}

void Servo_SetPulseUs(uint16_t pulse_us)
{
	Servo_SetPWM(Servo_PulseUsToCompare(pulse_us));
}

void Servo_RequestPulseUs(uint16_t pulse_us)
{
	servo_pending_pulse_us = Servo_ClampPulseUs(pulse_us);
	servo_update_pending = 1U;
}

void Servo_ProcessPending(void)
{
	uint16_t pulse_us;
	uint32_t primask;

	if (servo_update_pending == 0U) {
		return;
	}

	primask = __get_PRIMASK();
	__disable_irq();
	pulse_us = servo_pending_pulse_us;
	servo_update_pending = 0U;
	if (primask == 0U) {
		__enable_irq();
	}

	Servo_SetPulseUs(pulse_us);
}



