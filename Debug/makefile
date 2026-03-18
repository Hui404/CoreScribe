################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include sys/subdir.mk
-include grbl/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include MALLOC/subdir.mk
-include LVGL/lvgl/src/widgets/subdir.mk
-include LVGL/lvgl/src/misc/subdir.mk
-include LVGL/lvgl/src/hal/subdir.mk
-include LVGL/lvgl/src/font/subdir.mk
-include LVGL/lvgl/src/extra/widgets/win/subdir.mk
-include LVGL/lvgl/src/extra/widgets/tileview/subdir.mk
-include LVGL/lvgl/src/extra/widgets/tabview/subdir.mk
-include LVGL/lvgl/src/extra/widgets/spinner/subdir.mk
-include LVGL/lvgl/src/extra/widgets/spinbox/subdir.mk
-include LVGL/lvgl/src/extra/widgets/span/subdir.mk
-include LVGL/lvgl/src/extra/widgets/msgbox/subdir.mk
-include LVGL/lvgl/src/extra/widgets/meter/subdir.mk
-include LVGL/lvgl/src/extra/widgets/menu/subdir.mk
-include LVGL/lvgl/src/extra/widgets/list/subdir.mk
-include LVGL/lvgl/src/extra/widgets/led/subdir.mk
-include LVGL/lvgl/src/extra/widgets/keyboard/subdir.mk
-include LVGL/lvgl/src/extra/widgets/imgbtn/subdir.mk
-include LVGL/lvgl/src/extra/widgets/colorwheel/subdir.mk
-include LVGL/lvgl/src/extra/widgets/chart/subdir.mk
-include LVGL/lvgl/src/extra/widgets/calendar/subdir.mk
-include LVGL/lvgl/src/extra/widgets/animimg/subdir.mk
-include LVGL/lvgl/src/extra/themes/mono/subdir.mk
-include LVGL/lvgl/src/extra/themes/default/subdir.mk
-include LVGL/lvgl/src/extra/themes/basic/subdir.mk
-include LVGL/lvgl/src/extra/others/snapshot/subdir.mk
-include LVGL/lvgl/src/extra/others/monkey/subdir.mk
-include LVGL/lvgl/src/extra/others/gridnav/subdir.mk
-include LVGL/lvgl/src/extra/libs/sjpg/subdir.mk
-include LVGL/lvgl/src/extra/libs/rlottie/subdir.mk
-include LVGL/lvgl/src/extra/libs/qrcode/subdir.mk
-include LVGL/lvgl/src/extra/libs/png/subdir.mk
-include LVGL/lvgl/src/extra/libs/gif/subdir.mk
-include LVGL/lvgl/src/extra/libs/fsdrv/subdir.mk
-include LVGL/lvgl/src/extra/libs/freetype/subdir.mk
-include LVGL/lvgl/src/extra/libs/ffmpeg/subdir.mk
-include LVGL/lvgl/src/extra/libs/bmp/subdir.mk
-include LVGL/lvgl/src/extra/layouts/grid/subdir.mk
-include LVGL/lvgl/src/extra/layouts/flex/subdir.mk
-include LVGL/lvgl/src/extra/subdir.mk
-include LVGL/lvgl/src/draw/sw/subdir.mk
-include LVGL/lvgl/src/draw/stm32_dma2d/subdir.mk
-include LVGL/lvgl/src/draw/sdl/subdir.mk
-include LVGL/lvgl/src/draw/nxp_vglite/subdir.mk
-include LVGL/lvgl/src/draw/nxp_pxp/subdir.mk
-include LVGL/lvgl/src/draw/subdir.mk
-include LVGL/lvgl/src/core/subdir.mk
-include LVGL/lvgl/examples/porting/subdir.mk
-include LVGL/lvgl/demos/widgets/assets/subdir.mk
-include LVGL/lvgl/demos/widgets/subdir.mk
-include LVGL/lvgl/demos/stress/assets/subdir.mk
-include LVGL/lvgl/demos/stress/subdir.mk
-include LVGL/lvgl/demos/music/assets/subdir.mk
-include LVGL/lvgl/demos/music/subdir.mk
-include LVGL/lvgl/demos/keypad_encoder/subdir.mk
-include LVGL/lvgl/demos/benchmark/assets/subdir.mk
-include LVGL/lvgl/demos/benchmark/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := CoreScribe
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CoreScribe.elf \

MAP_FILES += \
CoreScribe.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CoreScribe.list \


# All Target
all: main-build

# Main-build Target
main-build: CoreScribe.elf secondary-outputs

# Tool invocations
CoreScribe.elf CoreScribe.map: $(OBJS) $(USER_OBJS) D:\STM32CubeIDE_Project\CoreScribe\STM32F407VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "CoreScribe.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\STM32CubeIDE_Project\CoreScribe\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CoreScribe.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CoreScribe.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CoreScribe.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) CoreScribe.elf CoreScribe.list CoreScribe.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
