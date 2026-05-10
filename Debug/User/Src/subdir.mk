################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/command.c \
../User/Src/hc_smu_control.c 

OBJS += \
./User/Src/command.o \
./User/Src/hc_smu_control.o 

C_DEPS += \
./User/Src/command.d \
./User/Src/hc_smu_control.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/%.o User/Src/%.su User/Src/%.cyclo: ../User/Src/%.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../User/Inc -I../User/Lib/FreeModbus/modbus -I../User/Lib/FreeModbus/port -I../User/Lib/FreeModbus/modbus/include -I../User/Lib/FreeModbus/modbus/ascii -I../User/Lib/FreeModbus/modbus/rtu -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/command.cyclo ./User/Src/command.d ./User/Src/command.o ./User/Src/command.su ./User/Src/hc_smu_control.cyclo ./User/Src/hc_smu_control.d ./User/Src/hc_smu_control.o ./User/Src/hc_smu_control.su

.PHONY: clean-User-2f-Src

