################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f407xx.s 

OBJS += \
./startup/startup_stm32f407xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I/home/n52/code_test/stm32f407/Inc -I/home/n52/code_test/stm32f407/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/n52/code_test/stm32f407/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/n52/code_test/stm32f407/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/n52/code_test/stm32f407/Drivers/CMSIS/Include -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/include -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/n52/code_test/stm32f407/DSP_Lib -I/home/n52/code_test/stm32f407/DSP_Lib/Include -I/home/n52/code_test/stm32f407/Debug/ -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


