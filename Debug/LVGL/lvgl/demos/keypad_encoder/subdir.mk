################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LVGL/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.c 

OBJS += \
./LVGL/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.o 

C_DEPS += \
./LVGL/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.d 


# Each subdirectory must supply rules for building sources it contributes
LVGL/lvgl/demos/keypad_encoder/%.o LVGL/lvgl/demos/keypad_encoder/%.su LVGL/lvgl/demos/keypad_encoder/%.cyclo: ../LVGL/lvgl/demos/keypad_encoder/%.c LVGL/lvgl/demos/keypad_encoder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../sys -I../MALLOC -I../LVGL -I../LVGL/lvgl -I../LVGL/lvgl/src -I../LVGL/lvgl/src/core -I../LVGL/lvgl/src/draw -I../LVGL/lvgl/src/extra -I../LVGL/lvgl/src/font -I../LVGL/lvgl/src/gpu -I../LVGL/lvgl/src/hal -I../LVGL/lvgl/src/misc -I../LVGL/lvgl/src/widgets -I../LVGL/lvgl/examples/porting -I../grbl -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LVGL/lvgl/demos/stress -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LVGL-2f-lvgl-2f-demos-2f-keypad_encoder

clean-LVGL-2f-lvgl-2f-demos-2f-keypad_encoder:
	-$(RM) ./LVGL/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.cyclo ./LVGL/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.d ./LVGL/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.o ./LVGL/lvgl/demos/keypad_encoder/lv_demo_keypad_encoder.su

.PHONY: clean-LVGL-2f-lvgl-2f-demos-2f-keypad_encoder

