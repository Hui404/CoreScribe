/*
  stepper.h - stepper motor driver: executes motion plans of planner.c using the stepper motors
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef stepper_h
#define stepper_h

#include "tim.h"
#include "gpio.h"

// 时钟转换常量
#define AVR_TIMER_CLOCK_HZ    2000000UL   // AVR Timer1时钟: 16MHz/8 = 2MHz
#define STM32_TIM2_CLOCK_HZ   2000000UL   // TIM2时钟: 84MHz/42 = 2MHz (保持一致)
#define CLOCK_RATIO           (STM32_TIM2_CLOCK_HZ / AVR_TIMER_CLOCK_HZ)  // 应该是1

// 脉冲宽度定时器配置（AVR Timer0是8分频，16MHz/8=2MHz）
#define AVR_TIMER0_CLOCK_HZ   2000000UL   // AVR Timer0时钟
#define STM32_TIM3_CLOCK_HZ   1000000UL   // TIM3时钟: 84MHz/84 = 1MHz


// 硬件抽象宏



#ifndef SEGMENT_BUFFER_SIZE
  #define SEGMENT_BUFFER_SIZE 6
#endif

// Initialize and setup the stepper motor subsystem
void stepper_init();

// Enable steppers, but cycle does not start unless called by motion control or realtime command.
void st_wake_up();

// Immediately disables steppers
void st_go_idle();
void st_finalize_idle();

// Generate the step and direction port invert masks.
void st_generate_step_dir_invert_masks();

// Reset the stepper subsystem variables
void st_reset();

// Changes the run state of the step segment buffer to execute the special parking motion.
void st_parking_setup_buffer();

// Restores the step segment buffer to the normal run state after a parking motion.
void st_parking_restore_buffer();

// Reloads step segment buffer. Called continuously by realtime execution system.
void st_prep_buffer();

// Called by planner_recalculate() when the executing block is updated by the new plan.
void st_update_plan_block_parameters();

// Called by realtime status reporting if realtime rate reporting is enabled in config.h.
float st_get_realtime_rate();

void stepper_timer_interrupt(void);
void pulse_width_timer_interrupt(void);

#endif
