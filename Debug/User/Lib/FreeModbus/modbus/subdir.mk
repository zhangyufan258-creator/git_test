################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Lib/FreeModbus/modbus/mb.c 

OBJS += \
./User/Lib/FreeModbus/modbus/mb.o 

C_DEPS += \
./User/Lib/FreeModbus/modbus/mb.d 


# Each subdirectory must supply rules for building sources it contributes
User/Lib/FreeModbus/modbus/%.o User/Lib/FreeModbus/modbus/%.su User/Lib/FreeModbus/modbus/%.cyclo: ../User/Lib/FreeModbus/modbus/%.c User/Lib/FreeModbus/modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../User/Inc -I../User/Lib/FreeModbus/modbus -I../User/Lib/FreeModbus/port -I../User/Lib/FreeModbus/modbus/include -I../User/Lib/FreeModbus/modbus/ascii -I../User/Lib/FreeModbus/modbus/rtu -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Lib-2f-FreeModbus-2f-modbus

clean-User-2f-Lib-2f-FreeModbus-2f-modbus:
	-$(RM) ./User/Lib/FreeModbus/modbus/mb.cyclo ./User/Lib/FreeModbus/modbus/mb.d ./User/Lib/FreeModbus/modbus/mb.o ./User/Lib/FreeModbus/modbus/mb.su

.PHONY: clean-User-2f-Lib-2f-FreeModbus-2f-modbus

