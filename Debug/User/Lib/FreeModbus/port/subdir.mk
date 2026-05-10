################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Lib/FreeModbus/port/port.c \
../User/Lib/FreeModbus/port/portevent.c \
../User/Lib/FreeModbus/port/portserial.c \
../User/Lib/FreeModbus/port/porttimer.c 

OBJS += \
./User/Lib/FreeModbus/port/port.o \
./User/Lib/FreeModbus/port/portevent.o \
./User/Lib/FreeModbus/port/portserial.o \
./User/Lib/FreeModbus/port/porttimer.o 

C_DEPS += \
./User/Lib/FreeModbus/port/port.d \
./User/Lib/FreeModbus/port/portevent.d \
./User/Lib/FreeModbus/port/portserial.d \
./User/Lib/FreeModbus/port/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
User/Lib/FreeModbus/port/%.o User/Lib/FreeModbus/port/%.su User/Lib/FreeModbus/port/%.cyclo: ../User/Lib/FreeModbus/port/%.c User/Lib/FreeModbus/port/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../User/Inc -I../User/Lib/FreeModbus/modbus -I../User/Lib/FreeModbus/port -I../User/Lib/FreeModbus/modbus/include -I../User/Lib/FreeModbus/modbus/ascii -I../User/Lib/FreeModbus/modbus/rtu -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Lib-2f-FreeModbus-2f-port

clean-User-2f-Lib-2f-FreeModbus-2f-port:
	-$(RM) ./User/Lib/FreeModbus/port/port.cyclo ./User/Lib/FreeModbus/port/port.d ./User/Lib/FreeModbus/port/port.o ./User/Lib/FreeModbus/port/port.su ./User/Lib/FreeModbus/port/portevent.cyclo ./User/Lib/FreeModbus/port/portevent.d ./User/Lib/FreeModbus/port/portevent.o ./User/Lib/FreeModbus/port/portevent.su ./User/Lib/FreeModbus/port/portserial.cyclo ./User/Lib/FreeModbus/port/portserial.d ./User/Lib/FreeModbus/port/portserial.o ./User/Lib/FreeModbus/port/portserial.su ./User/Lib/FreeModbus/port/porttimer.cyclo ./User/Lib/FreeModbus/port/porttimer.d ./User/Lib/FreeModbus/port/porttimer.o ./User/Lib/FreeModbus/port/porttimer.su

.PHONY: clean-User-2f-Lib-2f-FreeModbus-2f-port

