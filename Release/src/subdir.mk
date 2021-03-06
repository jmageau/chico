################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/attached_mode.c \
../src/behaviour.c \
../src/custom_timer.c \
../src/lcd.c \
../src/led.c \
../src/main.c \
../src/motion.c \
../src/ping.c \
../src/servo.c \
../src/tempsensor.c \
../src/timer.c \
../src/usart_serial.c \
../src/wireless_interface.c 

OBJS += \
./src/attached_mode.o \
./src/behaviour.o \
./src/custom_timer.o \
./src/lcd.o \
./src/led.o \
./src/main.o \
./src/motion.o \
./src/ping.o \
./src/servo.o \
./src/tempsensor.o \
./src/timer.o \
./src/usart_serial.o \
./src/wireless_interface.o 

C_DEPS += \
./src/attached_mode.d \
./src/behaviour.d \
./src/custom_timer.d \
./src/lcd.d \
./src/led.d \
./src/main.d \
./src/motion.d \
./src/ping.d \
./src/servo.d \
./src/tempsensor.d \
./src/timer.d \
./src/usart_serial.d \
./src/wireless_interface.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Nick\workspace\FreeRTOS801\include" -I"C:\Users\Nick\workspace\CEG4166_RTS_Lib\include" -DGCC_MEGA_AVR -Wall -O2 -ffunction-sections -fdata-sections -mcall-prologues -mrelax -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


