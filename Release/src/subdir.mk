################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lcd.c \
../src/lcdDisplay.c \
../src/led.c \
../src/main.c \
../src/motion.c \
../src/runtTimeStatsTimer.c \
../src/tempsensor.c 

OBJS += \
./src/lcd.o \
./src/lcdDisplay.o \
./src/led.o \
./src/main.o \
./src/motion.o \
./src/runtTimeStatsTimer.o \
./src/tempsensor.o 

C_DEPS += \
./src/lcd.d \
./src/lcdDisplay.d \
./src/led.d \
./src/main.d \
./src/motion.d \
./src/runtTimeStatsTimer.d \
./src/tempsensor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Nick\workspace\FreeRTOS801\include" -I"C:\Users\Nick\workspace\CEG4166_RTS_Lib\include" -DGCC_MEGA_AVR -Wall -O2 -ffunction-sections -fdata-sections -mcall-prologues -mrelax -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


