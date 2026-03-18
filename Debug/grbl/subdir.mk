################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../grbl/coolant_control.c \
../grbl/eeprom.c \
../grbl/gcode.c \
../grbl/grbl_limits.c \
../grbl/grbl_serial.c \
../grbl/grbl_start.c \
../grbl/jog.c \
../grbl/main.c \
../grbl/motion_control.c \
../grbl/nuts_bolts.c \
../grbl/planner.c \
../grbl/print.c \
../grbl/probe.c \
../grbl/protocol.c \
../grbl/report.c \
../grbl/settings.c \
../grbl/spindle_control.c \
../grbl/stepper.c \
../grbl/system.c 

OBJS += \
./grbl/coolant_control.o \
./grbl/eeprom.o \
./grbl/gcode.o \
./grbl/grbl_limits.o \
./grbl/grbl_serial.o \
./grbl/grbl_start.o \
./grbl/jog.o \
./grbl/main.o \
./grbl/motion_control.o \
./grbl/nuts_bolts.o \
./grbl/planner.o \
./grbl/print.o \
./grbl/probe.o \
./grbl/protocol.o \
./grbl/report.o \
./grbl/settings.o \
./grbl/spindle_control.o \
./grbl/stepper.o \
./grbl/system.o 

C_DEPS += \
./grbl/coolant_control.d \
./grbl/eeprom.d \
./grbl/gcode.d \
./grbl/grbl_limits.d \
./grbl/grbl_serial.d \
./grbl/grbl_start.d \
./grbl/jog.d \
./grbl/main.d \
./grbl/motion_control.d \
./grbl/nuts_bolts.d \
./grbl/planner.d \
./grbl/print.d \
./grbl/probe.d \
./grbl/protocol.d \
./grbl/report.d \
./grbl/settings.d \
./grbl/spindle_control.d \
./grbl/stepper.d \
./grbl/system.d 


# Each subdirectory must supply rules for building sources it contributes
grbl/%.o grbl/%.su grbl/%.cyclo: ../grbl/%.c grbl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../sys -I../MALLOC -I../LVGL -I../LVGL/lvgl -I../LVGL/lvgl/src -I../LVGL/lvgl/src/core -I../LVGL/lvgl/src/draw -I../LVGL/lvgl/src/extra -I../LVGL/lvgl/src/font -I../LVGL/lvgl/src/gpu -I../LVGL/lvgl/src/hal -I../LVGL/lvgl/src/misc -I../LVGL/lvgl/src/widgets -I../LVGL/lvgl/examples/porting -I../grbl -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LVGL/lvgl/demos/stress -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-grbl

clean-grbl:
	-$(RM) ./grbl/coolant_control.cyclo ./grbl/coolant_control.d ./grbl/coolant_control.o ./grbl/coolant_control.su ./grbl/eeprom.cyclo ./grbl/eeprom.d ./grbl/eeprom.o ./grbl/eeprom.su ./grbl/gcode.cyclo ./grbl/gcode.d ./grbl/gcode.o ./grbl/gcode.su ./grbl/grbl_limits.cyclo ./grbl/grbl_limits.d ./grbl/grbl_limits.o ./grbl/grbl_limits.su ./grbl/grbl_serial.cyclo ./grbl/grbl_serial.d ./grbl/grbl_serial.o ./grbl/grbl_serial.su ./grbl/grbl_start.cyclo ./grbl/grbl_start.d ./grbl/grbl_start.o ./grbl/grbl_start.su ./grbl/jog.cyclo ./grbl/jog.d ./grbl/jog.o ./grbl/jog.su ./grbl/main.cyclo ./grbl/main.d ./grbl/main.o ./grbl/main.su ./grbl/motion_control.cyclo ./grbl/motion_control.d ./grbl/motion_control.o ./grbl/motion_control.su ./grbl/nuts_bolts.cyclo ./grbl/nuts_bolts.d ./grbl/nuts_bolts.o ./grbl/nuts_bolts.su ./grbl/planner.cyclo ./grbl/planner.d ./grbl/planner.o ./grbl/planner.su ./grbl/print.cyclo ./grbl/print.d ./grbl/print.o ./grbl/print.su ./grbl/probe.cyclo ./grbl/probe.d ./grbl/probe.o ./grbl/probe.su ./grbl/protocol.cyclo ./grbl/protocol.d ./grbl/protocol.o ./grbl/protocol.su ./grbl/report.cyclo ./grbl/report.d ./grbl/report.o ./grbl/report.su ./grbl/settings.cyclo ./grbl/settings.d ./grbl/settings.o ./grbl/settings.su ./grbl/spindle_control.cyclo ./grbl/spindle_control.d ./grbl/spindle_control.o ./grbl/spindle_control.su ./grbl/stepper.cyclo ./grbl/stepper.d ./grbl/stepper.o ./grbl/stepper.su ./grbl/system.cyclo ./grbl/system.d ./grbl/system.o ./grbl/system.su

.PHONY: clean-grbl

