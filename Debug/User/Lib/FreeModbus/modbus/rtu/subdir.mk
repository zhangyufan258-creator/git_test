################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Lib/FreeModbus/modbus/rtu/mbcrc.c \
../User/Lib/FreeModbus/modbus/rtu/mbrtu.c 

OBJS += \
./User/Lib/FreeModbus/modbus/rtu/mbcrc.o \
./User/Lib/FreeModbus/modbus/rtu/mbrtu.o 

C_DEPS += \
./User/Lib/FreeModbus/modbus/rtu/mbcrc.d \
./User/Lib/FreeModbus/modbus/rtu/mbrtu.d 


# Each subdirectory must supply rules for building sources it contributes
User/Lib/FreeModbus/modbus/rtu/%.o User/Lib/FreeModbus/modbus/rtu/%.su User/Lib/FreeModbus/modbus/rtu/%.cyclo: ../User/Lib/FreeModbus/modbus/rtu/%.c User/Lib/FreeModbus/modbus/rtu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../User/Inc -I../User/Lib/FreeModbus/modbus -I../User/Lib/FreeModbus/port -I../User/Lib/FreeModbus/modbus/include -I../User/Lib/FreeModbus/modbus/ascii -I../User/Lib/FreeModbus/modbus/rtu -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Lib-2f-FreeModbus-2f-modbus-2f-rtu

clean-User-2f-Lib-2f-FreeModbus-2f-modbus-2f-rtu:
	-$(RM) ./User/Lib/FreeModbus/modbus/rtu/mbcrc.cyclo ./User/Lib/FreeModbus/modbus/rtu/mbcrc.d ./User/Lib/FreeModbus/modbus/rtu/mbcrc.o ./User/Lib/FreeModbus/modbus/rtu/mbcrc.su ./User/Lib/FreeModbus/modbus/rtu/mbrtu.cyclo ./User/Lib/FreeModbus/modbus/rtu/mbrtu.d ./User/Lib/FreeModbus/modbus/rtu/mbrtu.o ./User/Lib/FreeModbus/modbus/rtu/mbrtu.su

.PHONY: clean-User-2f-Lib-2f-FreeModbus-2f-modbus-2f-rtu

