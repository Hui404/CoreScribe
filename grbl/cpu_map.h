/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

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

/* The cpu_map.h files serve as a central pin mapping selection file for different
   processor types or alternative pin layouts. This version of Grbl officially supports
   only the Arduino Mega328p. */


#ifndef cpu_map_h
#define cpu_map_h

#include "stm32f4xx_hal.h"

#define STM32F407VET6

#ifdef STM32F407VET6 // (Arduino Uno) Officially supported by Grbl.

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  //定义步进脉冲输出引脚。注：所有的步进脉冲引脚必须定义在同一个端口上。
  #define STEP_GPIO_CLK_ENABLE()        	__HAL_RCC_GPIOA_CLK_ENABLE() //如果使用CubeIDE，按理说不用，但还是写上吧
  #define STEP_GPIO_PORT    				GPIOA
  #define X_STEP_Pin      					GPIO_PIN_1  // PA1
  #define Y_STEP_Pin      					GPIO_PIN_2  // PA2
  #define Z_STEP_Pin      					GPIO_PIN_3  // PA3
  //#define STEP_MASK   						(X_STEP_Pin | Y_STEP_Pin | Z_STEP_Pin)// All step bits

	  // 兼容原GRBL的位定义
	#define X_STEP_BIT      			2
	#define Y_STEP_BIT      			3
	#define Z_STEP_BIT      			4
	#define STEP_MASK       			((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) //此处为11100

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  //定义方向电平输出引脚。注：所有的方向引脚必须定义在同一个端口上。
  #define DIRECTION_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOA_CLK_ENABLE()
  #define DIRECTION_GPIO_PORT    			GPIOA
  #define X_DIRECTION_PIN        			GPIO_PIN_4      // PA4
  #define Y_DIRECTION_PIN        			GPIO_PIN_5      // PA5
  #define Z_DIRECTION_PIN       			GPIO_PIN_6      // PA6
  //#define DIRECTION_MASK       				(X_DIRECTION_PIN | Y_DIRECTION_PIN | Z_DIRECTION_PIN) // All direction bits
  // 兼容原GRBL的位定义
  #define X_DIRECTION_BIT   				5
  #define Y_DIRECTION_BIT   				6
  #define Z_DIRECTION_BIT   				7
  #define DIRECTION_MASK    				((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT))


  // Define stepper driver enable/disable output pin.
  //定义步进电机驱动器使能引脚
  #define STEPPERS_DISABLE_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
  #define STEPPERS_DISABLE_GPIO_PORT  			GPIOB
  #define STEPPERS_DISABLE_Pin    				GPIO_PIN_0 //PB0
  // 兼容原GRBL的位定义
  #define STEPPERS_DISABLE_BIT                  0
  #define STEPPERS_DISABLE_MASK                 (1 << STEPPERS_DISABLE_BIT)

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  //定义原点/硬限位开关输入引脚和限位中断向量
  //注意：所有的限位引脚必须使用同一个中断处理函数。
  #define LIMIT_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
  #define LIMIT_PORT       					GPIOC
  #define X_LIMIT_PIN                       GPIO_PIN_1  // PC1
  #define Y_LIMIT_PIN                       GPIO_PIN_2  // PC2
  #define Z_LIMIT_PIN                       GPIO_PIN_3  // PC3

  #define LIMIT_INT_PORTSOURCE 				GPIO_PortSourceGPIOC 	//中断源端口
  #define X_LIMIT_INT_PINSOURCE				GPIO_PinSource1	   	//中断源引脚 注：这三个为标准库风格
  #define Y_LIMIT_INT_PINSOURCE				GPIO_PinSource2		//中断源引脚
  #define Z_LIMIT_INT_PINSOURCE				GPIO_PinSource3		//中断源引脚
  #define X_LIMIT_INT_LINE					EXTI_LINE_1				//中断线
  #define Y_LIMIT_INT_LINE					EXTI_LINE_2				//中断线
  #define Z_LIMIT_INT_LINE					EXTI_LINE_3				//中断线
  #define X_LIMIT_EXTI_IRQn                 EXTI1_IRQn			    //中断请求号
  #define Y_LIMIT_EXTI_IRQn                 EXTI2_IRQn
  #define Z_LIMIT_EXTI_IRQn                 EXTI3_IRQn
  // 物理引脚掩码（用于GPIO操作）
  #define LIMIT_PINS_MASK                   (X_LIMIT_PIN | Y_LIMIT_PIN | Z_LIMIT_PIN)

  // 中断优先级（必须定义）
  #define LIMIT_IRQ_PRIORITY               0  // 最高优先级
  // 触发边沿（必须定义）
  #define LIMIT_EXTI_TRIGGER               EXTI_TRIGGER_FALLING  // 下降沿触发

  // 兼容原GRBL的位定义
  #define X_LIMIT_BIT      1
  #define Y_LIMIT_BIT      2
  #define Z_LIMIT_BIT      3
  #define LIMIT_MASK     ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
  #define CONTROL_GPIO_PORT              GPIOB
  #define CONTROL_RESET_PIN              GPIO_PIN_4  // PB4
  #define CONTROL_FEED_HOLD_PIN          GPIO_PIN_5  // PB5
  #define CONTROL_CYCLE_START_PIN        GPIO_PIN_6  // PB6
  #define CONTROL_SAFETY_DOOR_Pin 		 GPIO_PIN_7  // PB7

  #define CONTROL_INT_PORTSOURCE 		GPIO_PortSourceGPIOB 	//中断源端口
  #define RESET_INT_PINSOURCE			GPIO_PinSource4			//中断源引脚：复位
  #define FEED_HOLD_INT_PINSOURCE		GPIO_PinSource5			//中断源引脚：给进保持
  #define CYCLE_START_INT_PINSOURCE	    GPIO_PinSource6			//中断源引脚：循环开始
  #define SAFETY_DOOR_INT_PINSOURCE	    GPIO_PinSource7			//中断源引脚：紧急停车
  #define RESET_INT_LINE				EXTI_LINE_4				//中断线：复位
  #define FEED_HOLD_INT_LINE			EXTI_LINE_5				//中断线：给进保持
  #define CYCLE_START_INT_LINE		    EXTI_LINE_6				//中断线：循环开始
  #define SAFETY_DOOR_INT_LINE		    EXTI_LINE_7				//中断线：紧急停车

  // 中断请求号
  #define RESET_EXTI_IRQn                EXTI4_IRQn       // EXTI线4独立中断
  #define FEED_HOLD_EXTI_IRQn            EXTI9_5_IRQn     // EXTI线5-9共享中断
  #define CYCLE_START_EXTI_IRQn          EXTI9_5_IRQn     // 同上
  #define SAFETY_DOOR_EXTI_IRQn          EXTI9_5_IRQn     // 同上

  // 兼容原GRBL的位定义
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   3  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.





  // Define probe switch input pin.
  //定义探测开关输入引脚
  #define PROBE_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
  #define PROBE_Pin                     GPIO_PIN_1	//PB1
  #define PROBE_GPIO_Port 				GPIOB

  // 兼容原GRBL的位定义
  #define PROBE_BIT       5  // Uno Analog Pin 5
  #define PROBE_MASK      (1<<PROBE_BIT)

  // 写字机专用：抬笔/落笔控制
  #define PEN_SERVO_PWM_GPIO_Port    GPIOA
  #define PEN_SERVO_PWM_PIN          GPIO_PIN_8      // PA8，定时器1通道1
  #define PEN_UP_POSITION            2000           // 舵机角度：抬笔
  #define PEN_DOWN_POSITION          1000           // 舵机角度：落笔




#endif

/*
#ifdef CPU_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and edit one of the available cpu
  // map files and modify it to your needs. Make sure the defined name is also changed in
  // the config.h file.
#endif
*/

#endif
