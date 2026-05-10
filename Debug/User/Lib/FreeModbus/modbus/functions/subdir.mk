################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Lib/FreeModbus/modbus/functions/mbfunccoils.c \
../User/Lib/FreeModbus/modbus/functions/mbfuncdiag.c \
../User/Lib/FreeModbus/modbus/functions/mbfuncdisc.c \
../User/Lib/FreeModbus/modbus/functions/mbfuncholding.c \
../User/Lib/FreeModbus/modbus/functions/mbfuncinput.c \
../User/Lib/FreeModbus/modbus/functions/mbfuncother.c \
../User/Lib/FreeModbus/modbus/functions/mbutils.c 

OBJS += \
./User/Lib/FreeModbus/modbus/functions/mbfunccoils.o \
./User/Lib/FreeModbus/modbus/functions/mbfuncdiag.o \
./User/Lib/FreeModbus/modbus/functions/mbfuncdisc.o \
./User/Lib/FreeModbus/modbus/functions/mbfuncholding.o \
./User/Lib/FreeModbus/modbus/functions/mbfuncinput.o \
./User/Lib/FreeModbus/modbus/functions/mbfuncother.o \
./User/Lib/FreeModbus/modbus/functions/mbutils.o 

C_DEPS += \
./User/Lib/FreeModbus/modbus/functions/mbfunccoils.d \
./User/Lib/FreeModbus/modbus/functions/mbfuncdiag.d \
./User/Lib/FreeModbus/modbus/functions/mbfuncdisc.d \
./User/Lib/FreeModbus/modbus/functions/mbfuncholding.d \
./User/Lib/FreeModbus/modbus/functions/mbfuncinput.d \
./User/Lib/FreeModbus/modbus/functions/mbfuncother.d \
./User/Lib/FreeModbus/modbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
User/Lib/FreeModbus/modbus/functions/%.o User/Lib/FreeModbus/modbus/functions/%.su User/Lib/FreeModbus/modbus/functions/%.cyclo: ../User/Lib/FreeModbus/modbus/functions/%.c User/Lib/FreeModbus/modbus/functions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../User/Inc -I../User/Lib/FreeModbus/modbus -I../User/Lib/FreeModbus/port -I../User/Lib/FreeModbus/modbus/include -I../User/Lib/FreeModbus/modbus/ascii -I../User/Lib/FreeModbus/modbus/rtu -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-Lib-2f-FreeModbus-2f-modbus-2f-functions

clean-User-2f-Lib-2f-FreeModbus-2f-modbus-2f-functions:
	-$(RM) ./User/Lib/FreeModbus/modbus/functions/mbfunccoils.cyclo ./User/Lib/FreeModbus/modbus/functions/mbfunccoils.d ./User/Lib/FreeModbus/modbus/functions/mbfunccoils.o ./User/Lib/FreeModbus/modbus/functions/mbfunccoils.su ./User/Lib/FreeModbus/modbus/functions/mbfuncdiag.cyclo ./User/Lib/FreeModbus/modbus/functions/mbfuncdiag.d ./User/Lib/FreeModbus/modbus/functions/mbfuncdiag.o ./User/Lib/FreeModbus/modbus/functions/mbfuncdiag.su ./User/Lib/FreeModbus/modbus/functions/mbfuncdisc.cyclo ./User/Lib/FreeModbus/modbus/functions/mbfuncdisc.d ./User/Lib/FreeModbus/modbus/functions/mbfuncdisc.o ./User/Lib/FreeModbus/modbus/functions/mbfuncdisc.su ./User/Lib/FreeModbus/modbus/functions/mbfuncholding.cyclo ./User/Lib/FreeModbus/modbus/functions/mbfuncholding.d ./User/Lib/FreeModbus/modbus/functions/mbfuncholding.o ./User/Lib/FreeModbus/modbus/functions/mbfuncholding.su ./User/Lib/FreeModbus/modbus/functions/mbfuncinput.cyclo ./User/Lib/FreeModbus/modbus/functions/mbfuncinput.d ./User/Lib/FreeModbus/modbus/functions/mbfuncinput.o ./User/Lib/FreeModbus/modbus/functions/mbfuncinput.su ./User/Lib/FreeModbus/modbus/functions/mbfuncother.cyclo ./User/Lib/FreeModbus/modbus/functions/mbfuncother.d ./User/Lib/FreeModbus/modbus/functions/mbfuncother.o ./User/Lib/FreeModbus/modbus/functions/mbfuncother.su ./User/Lib/FreeModbus/modbus/functions/mbutils.cyclo ./User/Lib/FreeModbus/modbus/functions/mbutils.d ./User/Lib/FreeModbus/modbus/functions/mbutils.o ./User/Lib/FreeModbus/modbus/functions/mbutils.su

.PHONY: clean-User-2f-Lib-2f-FreeModbus-2f-modbus-2f-functions

