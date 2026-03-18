################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LVGL/lvgl/src/hal/lv_hal_disp.c \
../LVGL/lvgl/src/hal/lv_hal_indev.c \
../LVGL/lvgl/src/hal/lv_hal_tick.c 

OBJS += \
./LVGL/lvgl/src/hal/lv_hal_disp.o \
./LVGL/lvgl/src/hal/lv_hal_indev.o \
./LVGL/lvgl/src/hal/lv_hal_tick.o 

C_DEPS += \
./LVGL/lvgl/src/hal/lv_hal_disp.d \
./LVGL/lvgl/src/hal/lv_hal_indev.d \
./LVGL/lvgl/src/hal/lv_hal_tick.d 


# Each subdirectory must supply rules for building sources it contributes
LVGL/lvgl/src/hal/%.o LVGL/lvgl/src/hal/%.su LVGL/lvgl/src/hal/%.cyclo: ../LVGL/lvgl/src/hal/%.c LVGL/lvgl/src/hal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../sys -I../MALLOC -I../LVGL -I../LVGL/lvgl -I../LVGL/lvgl/src -I../LVGL/lvgl/src/core -I../LVGL/lvgl/src/draw -I../LVGL/lvgl/src/extra -I../LVGL/lvgl/src/font -I../LVGL/lvgl/src/gpu -I../LVGL/lvgl/src/hal -I../LVGL/lvgl/src/misc -I../LVGL/lvgl/src/widgets -I../LVGL/lvgl/examples/porting -I../grbl -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LVGL/lvgl/demos/stress -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LVGL-2f-lvgl-2f-src-2f-hal

clean-LVGL-2f-lvgl-2f-src-2f-hal:
	-$(RM) ./LVGL/lvgl/src/hal/lv_hal_disp.cyclo ./LVGL/lvgl/src/hal/lv_hal_disp.d ./LVGL/lvgl/src/hal/lv_hal_disp.o ./LVGL/lvgl/src/hal/lv_hal_disp.su ./LVGL/lvgl/src/hal/lv_hal_indev.cyclo ./LVGL/lvgl/src/hal/lv_hal_indev.d ./LVGL/lvgl/src/hal/lv_hal_indev.o ./LVGL/lvgl/src/hal/lv_hal_indev.su ./LVGL/lvgl/src/hal/lv_hal_tick.cyclo ./LVGL/lvgl/src/hal/lv_hal_tick.d ./LVGL/lvgl/src/hal/lv_hal_tick.o ./LVGL/lvgl/src/hal/lv_hal_tick.su

.PHONY: clean-LVGL-2f-lvgl-2f-src-2f-hal

