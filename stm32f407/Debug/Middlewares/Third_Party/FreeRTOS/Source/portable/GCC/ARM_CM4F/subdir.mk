################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F407xx -D__FPU_USED -D__FPU_PRESENT -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -I/home/n52/code_test/stm32f407/Inc -I/home/n52/code_test/stm32f407/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/n52/code_test/stm32f407/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/n52/code_test/stm32f407/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/n52/code_test/stm32f407/Drivers/CMSIS/Include -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/include -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/n52/code_test/stm32f407/DSP_Lib -I/home/n52/code_test/stm32f407/DSP_Lib/Include -I/home/n52/code_test/stm32f407/Debug/ -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


