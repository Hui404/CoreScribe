################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LVGL/lvgl/src/core/lv_disp.c \
../LVGL/lvgl/src/core/lv_event.c \
../LVGL/lvgl/src/core/lv_group.c \
../LVGL/lvgl/src/core/lv_indev.c \
../LVGL/lvgl/src/core/lv_indev_scroll.c \
../LVGL/lvgl/src/core/lv_obj.c \
../LVGL/lvgl/src/core/lv_obj_class.c \
../LVGL/lvgl/src/core/lv_obj_draw.c \
../LVGL/lvgl/src/core/lv_obj_pos.c \
../LVGL/lvgl/src/core/lv_obj_scroll.c \
../LVGL/lvgl/src/core/lv_obj_style.c \
../LVGL/lvgl/src/core/lv_obj_style_gen.c \
../LVGL/lvgl/src/core/lv_obj_tree.c \
../LVGL/lvgl/src/core/lv_refr.c \
../LVGL/lvgl/src/core/lv_theme.c 

OBJS += \
./LVGL/lvgl/src/core/lv_disp.o \
./LVGL/lvgl/src/core/lv_event.o \
./LVGL/lvgl/src/core/lv_group.o \
./LVGL/lvgl/src/core/lv_indev.o \
./LVGL/lvgl/src/core/lv_indev_scroll.o \
./LVGL/lvgl/src/core/lv_obj.o \
./LVGL/lvgl/src/core/lv_obj_class.o \
./LVGL/lvgl/src/core/lv_obj_draw.o \
./LVGL/lvgl/src/core/lv_obj_pos.o \
./LVGL/lvgl/src/core/lv_obj_scroll.o \
./LVGL/lvgl/src/core/lv_obj_style.o \
./LVGL/lvgl/src/core/lv_obj_style_gen.o \
./LVGL/lvgl/src/core/lv_obj_tree.o \
./LVGL/lvgl/src/core/lv_refr.o \
./LVGL/lvgl/src/core/lv_theme.o 

C_DEPS += \
./LVGL/lvgl/src/core/lv_disp.d \
./LVGL/lvgl/src/core/lv_event.d \
./LVGL/lvgl/src/core/lv_group.d \
./LVGL/lvgl/src/core/lv_indev.d \
./LVGL/lvgl/src/core/lv_indev_scroll.d \
./LVGL/lvgl/src/core/lv_obj.d \
./LVGL/lvgl/src/core/lv_obj_class.d \
./LVGL/lvgl/src/core/lv_obj_draw.d \
./LVGL/lvgl/src/core/lv_obj_pos.d \
./LVGL/lvgl/src/core/lv_obj_scroll.d \
./LVGL/lvgl/src/core/lv_obj_style.d \
./LVGL/lvgl/src/core/lv_obj_style_gen.d \
./LVGL/lvgl/src/core/lv_obj_tree.d \
./LVGL/lvgl/src/core/lv_refr.d \
./LVGL/lvgl/src/core/lv_theme.d 


# Each subdirectory must supply rules for building sources it contributes
LVGL/lvgl/src/core/%.o LVGL/lvgl/src/core/%.su LVGL/lvgl/src/core/%.cyclo: ../LVGL/lvgl/src/core/%.c LVGL/lvgl/src/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../sys -I../MALLOC -I../LVGL -I../LVGL/lvgl -I../LVGL/lvgl/src -I../LVGL/lvgl/src/core -I../LVGL/lvgl/src/draw -I../LVGL/lvgl/src/extra -I../LVGL/lvgl/src/font -I../LVGL/lvgl/src/gpu -I../LVGL/lvgl/src/hal -I../LVGL/lvgl/src/misc -I../LVGL/lvgl/src/widgets -I../LVGL/lvgl/examples/porting -I../grbl -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LVGL/lvgl/demos/stress -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LVGL-2f-lvgl-2f-src-2f-core

clean-LVGL-2f-lvgl-2f-src-2f-core:
	-$(RM) ./LVGL/lvgl/src/core/lv_disp.cyclo ./LVGL/lvgl/src/core/lv_disp.d ./LVGL/lvgl/src/core/lv_disp.o ./LVGL/lvgl/src/core/lv_disp.su ./LVGL/lvgl/src/core/lv_event.cyclo ./LVGL/lvgl/src/core/lv_event.d ./LVGL/lvgl/src/core/lv_event.o ./LVGL/lvgl/src/core/lv_event.su ./LVGL/lvgl/src/core/lv_group.cyclo ./LVGL/lvgl/src/core/lv_group.d ./LVGL/lvgl/src/core/lv_group.o ./LVGL/lvgl/src/core/lv_group.su ./LVGL/lvgl/src/core/lv_indev.cyclo ./LVGL/lvgl/src/core/lv_indev.d ./LVGL/lvgl/src/core/lv_indev.o ./LVGL/lvgl/src/core/lv_indev.su ./LVGL/lvgl/src/core/lv_indev_scroll.cyclo ./LVGL/lvgl/src/core/lv_indev_scroll.d ./LVGL/lvgl/src/core/lv_indev_scroll.o ./LVGL/lvgl/src/core/lv_indev_scroll.su ./LVGL/lvgl/src/core/lv_obj.cyclo ./LVGL/lvgl/src/core/lv_obj.d ./LVGL/lvgl/src/core/lv_obj.o ./LVGL/lvgl/src/core/lv_obj.su ./LVGL/lvgl/src/core/lv_obj_class.cyclo ./LVGL/lvgl/src/core/lv_obj_class.d ./LVGL/lvgl/src/core/lv_obj_class.o ./LVGL/lvgl/src/core/lv_obj_class.su ./LVGL/lvgl/src/core/lv_obj_draw.cyclo ./LVGL/lvgl/src/core/lv_obj_draw.d ./LVGL/lvgl/src/core/lv_obj_draw.o ./LVGL/lvgl/src/core/lv_obj_draw.su ./LVGL/lvgl/src/core/lv_obj_pos.cyclo ./LVGL/lvgl/src/core/lv_obj_pos.d ./LVGL/lvgl/src/core/lv_obj_pos.o ./LVGL/lvgl/src/core/lv_obj_pos.su ./LVGL/lvgl/src/core/lv_obj_scroll.cyclo ./LVGL/lvgl/src/core/lv_obj_scroll.d ./LVGL/lvgl/src/core/lv_obj_scroll.o ./LVGL/lvgl/src/core/lv_obj_scroll.su ./LVGL/lvgl/src/core/lv_obj_style.cyclo ./LVGL/lvgl/src/core/lv_obj_style.d ./LVGL/lvgl/src/core/lv_obj_style.o ./LVGL/lvgl/src/core/lv_obj_style.su ./LVGL/lvgl/src/core/lv_obj_style_gen.cyclo ./LVGL/lvgl/src/core/lv_obj_style_gen.d ./LVGL/lvgl/src/core/lv_obj_style_gen.o ./LVGL/lvgl/src/core/lv_obj_style_gen.su ./LVGL/lvgl/src/core/lv_obj_tree.cyclo ./LVGL/lvgl/src/core/lv_obj_tree.d ./LVGL/lvgl/src/core/lv_obj_tree.o ./LVGL/lvgl/src/core/lv_obj_tree.su ./LVGL/lvgl/src/core/lv_refr.cyclo ./LVGL/lvgl/src/core/lv_refr.d ./LVGL/lvgl/src/core/lv_refr.o ./LVGL/lvgl/src/core/lv_refr.su ./LVGL/lvgl/src/core/lv_theme.cyclo ./LVGL/lvgl/src/core/lv_theme.d ./LVGL/lvgl/src/core/lv_theme.o ./LVGL/lvgl/src/core/lv_theme.su

.PHONY: clean-LVGL-2f-lvgl-2f-src-2f-core

