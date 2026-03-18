################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LVGL/lvgl/demos/stress/assets/lv_font_montserrat_12_compr_az.c \
../LVGL/lvgl/demos/stress/assets/lv_font_montserrat_16_compr_az.c \
../LVGL/lvgl/demos/stress/assets/lv_font_montserrat_28_compr_az.c 

OBJS += \
./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_12_compr_az.o \
./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_16_compr_az.o \
./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_28_compr_az.o 

C_DEPS += \
./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_12_compr_az.d \
./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_16_compr_az.d \
./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_28_compr_az.d 


# Each subdirectory must supply rules for building sources it contributes
LVGL/lvgl/demos/stress/assets/%.o LVGL/lvgl/demos/stress/assets/%.su LVGL/lvgl/demos/stress/assets/%.cyclo: ../LVGL/lvgl/demos/stress/assets/%.c LVGL/lvgl/demos/stress/assets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../sys -I../MALLOC -I../LVGL -I../LVGL/lvgl -I../LVGL/lvgl/src -I../LVGL/lvgl/src/core -I../LVGL/lvgl/src/draw -I../LVGL/lvgl/src/extra -I../LVGL/lvgl/src/font -I../LVGL/lvgl/src/gpu -I../LVGL/lvgl/src/hal -I../LVGL/lvgl/src/misc -I../LVGL/lvgl/src/widgets -I../LVGL/lvgl/examples/porting -I../grbl -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LVGL/lvgl/demos/stress -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LVGL-2f-lvgl-2f-demos-2f-stress-2f-assets

clean-LVGL-2f-lvgl-2f-demos-2f-stress-2f-assets:
	-$(RM) ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_12_compr_az.cyclo ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_12_compr_az.d ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_12_compr_az.o ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_12_compr_az.su ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_16_compr_az.cyclo ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_16_compr_az.d ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_16_compr_az.o ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_16_compr_az.su ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_28_compr_az.cyclo ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_28_compr_az.d ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_28_compr_az.o ./LVGL/lvgl/demos/stress/assets/lv_font_montserrat_28_compr_az.su

.PHONY: clean-LVGL-2f-lvgl-2f-demos-2f-stress-2f-assets

