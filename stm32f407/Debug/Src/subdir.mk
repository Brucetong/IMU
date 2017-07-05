################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/AHRS_matrix.c \
../Src/AHRS_quat.c \
../Src/AHRS_states.c \
../Src/IMU.c \
../Src/delay.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/led.c \
../Src/main.c \
../Src/math_nb.c \
../Src/mpu6500.c \
../Src/pid.c \
../Src/send_data.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/sys.c \
../Src/syscalls.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/usart.c \
../Src/utask.c 

OBJS += \
./Src/AHRS_matrix.o \
./Src/AHRS_quat.o \
./Src/AHRS_states.o \
./Src/IMU.o \
./Src/delay.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/led.o \
./Src/main.o \
./Src/math_nb.o \
./Src/mpu6500.o \
./Src/pid.o \
./Src/send_data.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/sys.o \
./Src/syscalls.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/usart.o \
./Src/utask.o 

C_DEPS += \
./Src/AHRS_matrix.d \
./Src/AHRS_quat.d \
./Src/AHRS_states.d \
./Src/IMU.d \
./Src/delay.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/led.d \
./Src/main.d \
./Src/math_nb.d \
./Src/mpu6500.d \
./Src/pid.d \
./Src/send_data.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/sys.d \
./Src/syscalls.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/usart.d \
./Src/utask.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F407xx -D__FPU_USED -D__FPU_PRESENT -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -I/home/n52/code_test/stm32f407/Inc -I/home/n52/code_test/stm32f407/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/n52/code_test/stm32f407/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/n52/code_test/stm32f407/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/n52/code_test/stm32f407/Drivers/CMSIS/Include -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/include -I/home/n52/code_test/stm32f407/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/n52/code_test/stm32f407/DSP_Lib -I/home/n52/code_test/stm32f407/DSP_Lib/Include -I/home/n52/code_test/stm32f407/Debug/ -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


