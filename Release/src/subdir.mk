################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/behaviour.c \
../src/lcd.c \
../src/led.c \
../src/main.c \
../src/motion.c \
../src/servo.c \
../src/tempsensor.c \
../src/timer.c 

OBJS += \
./src/behaviour.o \
./src/lcd.o \
./src/led.o \
./src/main.o \
./src/motion.o \
./src/servo.o \
./src/tempsensor.o \
./src/timer.o 

C_DEPS += \
./src/behaviour.d \
./src/lcd.d \
./src/led.d \
./src/main.d \
./src/motion.d \
./src/servo.d \
./src/tempsensor.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Nick\workspace\FreeRTOS801\include" -I"C:\Users\Nick\workspace\CEG4166_RTS_Lib\include" -DGCC_MEGA_AVR -Wall -O2 -ffunction-sections -fdata-sections -mcall-prologues -mrelax -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


