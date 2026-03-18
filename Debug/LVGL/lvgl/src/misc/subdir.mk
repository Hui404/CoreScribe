################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LVGL/lvgl/src/misc/lv_anim.c \
../LVGL/lvgl/src/misc/lv_anim_timeline.c \
../LVGL/lvgl/src/misc/lv_area.c \
../LVGL/lvgl/src/misc/lv_async.c \
../LVGL/lvgl/src/misc/lv_bidi.c \
../LVGL/lvgl/src/misc/lv_color.c \
../LVGL/lvgl/src/misc/lv_fs.c \
../LVGL/lvgl/src/misc/lv_gc.c \
../LVGL/lvgl/src/misc/lv_ll.c \
../LVGL/lvgl/src/misc/lv_log.c \
../LVGL/lvgl/src/misc/lv_lru.c \
../LVGL/lvgl/src/misc/lv_math.c \
../LVGL/lvgl/src/misc/lv_mem.c \
../LVGL/lvgl/src/misc/lv_printf.c \
../LVGL/lvgl/src/misc/lv_style.c \
../LVGL/lvgl/src/misc/lv_style_gen.c \
../LVGL/lvgl/src/misc/lv_templ.c \
../LVGL/lvgl/src/misc/lv_timer.c \
../LVGL/lvgl/src/misc/lv_tlsf.c \
../LVGL/lvgl/src/misc/lv_txt.c \
../LVGL/lvgl/src/misc/lv_txt_ap.c \
../LVGL/lvgl/src/misc/lv_utils.c 

OBJS += \
./LVGL/lvgl/src/misc/lv_anim.o \
./LVGL/lvgl/src/misc/lv_anim_timeline.o \
./LVGL/lvgl/src/misc/lv_area.o \
./LVGL/lvgl/src/misc/lv_async.o \
./LVGL/lvgl/src/misc/lv_bidi.o \
./LVGL/lvgl/src/misc/lv_color.o \
./LVGL/lvgl/src/misc/lv_fs.o \
./LVGL/lvgl/src/misc/lv_gc.o \
./LVGL/lvgl/src/misc/lv_ll.o \
./LVGL/lvgl/src/misc/lv_log.o \
./LVGL/lvgl/src/misc/lv_lru.o \
./LVGL/lvgl/src/misc/lv_math.o \
./LVGL/lvgl/src/misc/lv_mem.o \
./LVGL/lvgl/src/misc/lv_printf.o \
./LVGL/lvgl/src/misc/lv_style.o \
./LVGL/lvgl/src/misc/lv_style_gen.o \
./LVGL/lvgl/src/misc/lv_templ.o \
./LVGL/lvgl/src/misc/lv_timer.o \
./LVGL/lvgl/src/misc/lv_tlsf.o \
./LVGL/lvgl/src/misc/lv_txt.o \
./LVGL/lvgl/src/misc/lv_txt_ap.o \
./LVGL/lvgl/src/misc/lv_utils.o 

C_DEPS += \
./LVGL/lvgl/src/misc/lv_anim.d \
./LVGL/lvgl/src/misc/lv_anim_timeline.d \
./LVGL/lvgl/src/misc/lv_area.d \
./LVGL/lvgl/src/misc/lv_async.d \
./LVGL/lvgl/src/misc/lv_bidi.d \
./LVGL/lvgl/src/misc/lv_color.d \
./LVGL/lvgl/src/misc/lv_fs.d \
./LVGL/lvgl/src/misc/lv_gc.d \
./LVGL/lvgl/src/misc/lv_ll.d \
./LVGL/lvgl/src/misc/lv_log.d \
./LVGL/lvgl/src/misc/lv_lru.d \
./LVGL/lvgl/src/misc/lv_math.d \
./LVGL/lvgl/src/misc/lv_mem.d \
./LVGL/lvgl/src/misc/lv_printf.d \
./LVGL/lvgl/src/misc/lv_style.d \
./LVGL/lvgl/src/misc/lv_style_gen.d \
./LVGL/lvgl/src/misc/lv_templ.d \
./LVGL/lvgl/src/misc/lv_timer.d \
./LVGL/lvgl/src/misc/lv_tlsf.d \
./LVGL/lvgl/src/misc/lv_txt.d \
./LVGL/lvgl/src/misc/lv_txt_ap.d \
./LVGL/lvgl/src/misc/lv_utils.d 


# Each subdirectory must supply rules for building sources it contributes
LVGL/lvgl/src/misc/%.o LVGL/lvgl/src/misc/%.su LVGL/lvgl/src/misc/%.cyclo: ../LVGL/lvgl/src/misc/%.c LVGL/lvgl/src/misc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../sys -I../MALLOC -I../LVGL -I../LVGL/lvgl -I../LVGL/lvgl/src -I../LVGL/lvgl/src/core -I../LVGL/lvgl/src/draw -I../LVGL/lvgl/src/extra -I../LVGL/lvgl/src/font -I../LVGL/lvgl/src/gpu -I../LVGL/lvgl/src/hal -I../LVGL/lvgl/src/misc -I../LVGL/lvgl/src/widgets -I../LVGL/lvgl/examples/porting -I../grbl -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LVGL/lvgl/demos/stress -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LVGL-2f-lvgl-2f-src-2f-misc

clean-LVGL-2f-lvgl-2f-src-2f-misc:
	-$(RM) ./LVGL/lvgl/src/misc/lv_anim.cyclo ./LVGL/lvgl/src/misc/lv_anim.d ./LVGL/lvgl/src/misc/lv_anim.o ./LVGL/lvgl/src/misc/lv_anim.su ./LVGL/lvgl/src/misc/lv_anim_timeline.cyclo ./LVGL/lvgl/src/misc/lv_anim_timeline.d ./LVGL/lvgl/src/misc/lv_anim_timeline.o ./LVGL/lvgl/src/misc/lv_anim_timeline.su ./LVGL/lvgl/src/misc/lv_area.cyclo ./LVGL/lvgl/src/misc/lv_area.d ./LVGL/lvgl/src/misc/lv_area.o ./LVGL/lvgl/src/misc/lv_area.su ./LVGL/lvgl/src/misc/lv_async.cyclo ./LVGL/lvgl/src/misc/lv_async.d ./LVGL/lvgl/src/misc/lv_async.o ./LVGL/lvgl/src/misc/lv_async.su ./LVGL/lvgl/src/misc/lv_bidi.cyclo ./LVGL/lvgl/src/misc/lv_bidi.d ./LVGL/lvgl/src/misc/lv_bidi.o ./LVGL/lvgl/src/misc/lv_bidi.su ./LVGL/lvgl/src/misc/lv_color.cyclo ./LVGL/lvgl/src/misc/lv_color.d ./LVGL/lvgl/src/misc/lv_color.o ./LVGL/lvgl/src/misc/lv_color.su ./LVGL/lvgl/src/misc/lv_fs.cyclo ./LVGL/lvgl/src/misc/lv_fs.d ./LVGL/lvgl/src/misc/lv_fs.o ./LVGL/lvgl/src/misc/lv_fs.su ./LVGL/lvgl/src/misc/lv_gc.cyclo ./LVGL/lvgl/src/misc/lv_gc.d ./LVGL/lvgl/src/misc/lv_gc.o ./LVGL/lvgl/src/misc/lv_gc.su ./LVGL/lvgl/src/misc/lv_ll.cyclo ./LVGL/lvgl/src/misc/lv_ll.d ./LVGL/lvgl/src/misc/lv_ll.o ./LVGL/lvgl/src/misc/lv_ll.su ./LVGL/lvgl/src/misc/lv_log.cyclo ./LVGL/lvgl/src/misc/lv_log.d ./LVGL/lvgl/src/misc/lv_log.o ./LVGL/lvgl/src/misc/lv_log.su ./LVGL/lvgl/src/misc/lv_lru.cyclo ./LVGL/lvgl/src/misc/lv_lru.d ./LVGL/lvgl/src/misc/lv_lru.o ./LVGL/lvgl/src/misc/lv_lru.su ./LVGL/lvgl/src/misc/lv_math.cyclo ./LVGL/lvgl/src/misc/lv_math.d ./LVGL/lvgl/src/misc/lv_math.o ./LVGL/lvgl/src/misc/lv_math.su ./LVGL/lvgl/src/misc/lv_mem.cyclo ./LVGL/lvgl/src/misc/lv_mem.d ./LVGL/lvgl/src/misc/lv_mem.o ./LVGL/lvgl/src/misc/lv_mem.su ./LVGL/lvgl/src/misc/lv_printf.cyclo ./LVGL/lvgl/src/misc/lv_printf.d ./LVGL/lvgl/src/misc/lv_printf.o ./LVGL/lvgl/src/misc/lv_printf.su ./LVGL/lvgl/src/misc/lv_style.cyclo ./LVGL/lvgl/src/misc/lv_style.d ./LVGL/lvgl/src/misc/lv_style.o ./LVGL/lvgl/src/misc/lv_style.su ./LVGL/lvgl/src/misc/lv_style_gen.cyclo ./LVGL/lvgl/src/misc/lv_style_gen.d ./LVGL/lvgl/src/misc/lv_style_gen.o ./LVGL/lvgl/src/misc/lv_style_gen.su ./LVGL/lvgl/src/misc/lv_templ.cyclo ./LVGL/lvgl/src/misc/lv_templ.d ./LVGL/lvgl/src/misc/lv_templ.o ./LVGL/lvgl/src/misc/lv_templ.su ./LVGL/lvgl/src/misc/lv_timer.cyclo ./LVGL/lvgl/src/misc/lv_timer.d ./LVGL/lvgl/src/misc/lv_timer.o ./LVGL/lvgl/src/misc/lv_timer.su ./LVGL/lvgl/src/misc/lv_tlsf.cyclo ./LVGL/lvgl/src/misc/lv_tlsf.d ./LVGL/lvgl/src/misc/lv_tlsf.o ./LVGL/lvgl/src/misc/lv_tlsf.su ./LVGL/lvgl/src/misc/lv_txt.cyclo ./LVGL/lvgl/src/misc/lv_txt.d ./LVGL/lvgl/src/misc/lv_txt.o ./LVGL/lvgl/src/misc/lv_txt.su ./LVGL/lvgl/src/misc/lv_txt_ap.cyclo ./LVGL/lvgl/src/misc/lv_txt_ap.d ./LVGL/lvgl/src/misc/lv_txt_ap.o ./LVGL/lvgl/src/misc/lv_txt_ap.su ./LVGL/lvgl/src/misc/lv_utils.cyclo ./LVGL/lvgl/src/misc/lv_utils.d ./LVGL/lvgl/src/misc/lv_utils.o ./LVGL/lvgl/src/misc/lv_utils.su

.PHONY: clean-LVGL-2f-lvgl-2f-src-2f-misc

