################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/Src/subdir.mk
-include User/Lib/FreeModbus/port/subdir.mk
-include User/Lib/FreeModbus/modbus/tcp/subdir.mk
-include User/Lib/FreeModbus/modbus/rtu/subdir.mk
-include User/Lib/FreeModbus/modbus/functions/subdir.mk
-include User/Lib/FreeModbus/modbus/ascii/subdir.mk
-include User/Lib/FreeModbus/modbus/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := Four_CH_CalibrationBoard
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Four_CH_CalibrationBoard.elf \

MAP_FILES += \
Four_CH_CalibrationBoard.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Four_CH_CalibrationBoard.list \


# All Target
all: main-build

# Main-build Target
main-build: Four_CH_CalibrationBoard.elf secondary-outputs

# Tool invocations
Four_CH_CalibrationBoard.elf Four_CH_CalibrationBoard.map: $(OBJS) $(USER_OBJS) D:\Document\艾宇学习资料\Four_CH_CalibrationBoard\Four_CH_CalibrationBoard\Four_CH_CalibrationBoard\STM32F103RCTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Four_CH_CalibrationBoard.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\Document\艾宇学习资料\Four_CH_CalibrationBoard\Four_CH_CalibrationBoard\Four_CH_CalibrationBoard\STM32F103RCTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Four_CH_CalibrationBoard.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Four_CH_CalibrationBoard.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Four_CH_CalibrationBoard.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Four_CH_CalibrationBoard.elf Four_CH_CalibrationBoard.list Four_CH_CalibrationBoard.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
