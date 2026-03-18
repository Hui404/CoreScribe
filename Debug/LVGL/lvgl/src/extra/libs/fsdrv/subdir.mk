################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.c \
../LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_posix.c \
../LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.c \
../LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_win32.c 

OBJS += \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.o \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_posix.o \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.o \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_win32.o 

C_DEPS += \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.d \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_posix.d \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.d \
./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_win32.d 


# Each subdirectory must supply rules for building sources it contributes
LVGL/lvgl/src/extra/libs/fsdrv/%.o LVGL/lvgl/src/extra/libs/fsdrv/%.su LVGL/lvgl/src/extra/libs/fsdrv/%.cyclo: ../LVGL/lvgl/src/extra/libs/fsdrv/%.c LVGL/lvgl/src/extra/libs/fsdrv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../sys -I../MALLOC -I../LVGL -I../LVGL/lvgl -I../LVGL/lvgl/src -I../LVGL/lvgl/src/core -I../LVGL/lvgl/src/draw -I../LVGL/lvgl/src/extra -I../LVGL/lvgl/src/font -I../LVGL/lvgl/src/gpu -I../LVGL/lvgl/src/hal -I../LVGL/lvgl/src/misc -I../LVGL/lvgl/src/widgets -I../LVGL/lvgl/examples/porting -I../grbl -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LVGL/lvgl/demos/stress -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LVGL-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv

clean-LVGL-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv:
	-$(RM) ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.cyclo ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.d ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.o ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.su ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_posix.cyclo ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_posix.d ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_posix.o ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_posix.su ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.cyclo ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.d ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.o ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.su ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_win32.cyclo ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_win32.d ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_win32.o ./LVGL/lvgl/src/extra/libs/fsdrv/lv_fs_win32.su

.PHONY: clean-LVGL-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv

