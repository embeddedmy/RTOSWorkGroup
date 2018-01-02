################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/HTS221.c \
../Src/data.c \
../Src/freertos.c \
../Src/main.c \
../Src/sensor.c \
../Src/stm32l0xx_hal_msp.c \
../Src/stm32l0xx_it.c \
../Src/system_stm32l0xx.c \
../Src/transmit.c 

OBJS += \
./Src/HTS221.o \
./Src/data.o \
./Src/freertos.o \
./Src/main.o \
./Src/sensor.o \
./Src/stm32l0xx_hal_msp.o \
./Src/stm32l0xx_it.o \
./Src/system_stm32l0xx.o \
./Src/transmit.o 

C_DEPS += \
./Src/HTS221.d \
./Src/data.d \
./Src/freertos.d \
./Src/main.d \
./Src/sensor.d \
./Src/stm32l0xx_hal_msp.d \
./Src/stm32l0xx_it.d \
./Src/system_stm32l0xx.d \
./Src/transmit.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L053xx -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Inc" -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Drivers/STM32L0xx_HAL_Driver/Inc" -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Drivers/STM32L0xx_HAL_Driver/Inc/Legacy" -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0" -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Drivers/CMSIS/Device/ST/STM32L0xx/Include" -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/LK/Documents/RTOS_workgroup/RTOSWG_TEST/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


