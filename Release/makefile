################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
EmptyProject.lss \

FLASH_IMAGE += \
EmptyProject.hex \

EEPROM_IMAGE += \
EmptyProject.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: EmptyProject.elf secondary-outputs

dependents:
	-cd C:/Users/Nick/workspace/FreeRTOS801/Release && $(MAKE) all
	-cd C:/Users/Nick/workspace/CEG4166_RTS_Lib/Release && $(MAKE) all

# Tool invocations
EmptyProject.elf: $(OBJS) $(USER_OBJS) C:/Users/Nick/workspace/FreeRTOS801/Release/libFreeRTOS801.a C:/Users/Nick/workspace/CEG4166_RTS_Lib/Release/libCEG4166_RTS_Lib.a
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,EmptyProject.map --gc-sections -Wl,-u,vfprintf -Wl -L"C:\Users\Nick\workspace\FreeRTOS801\Release" -L"C:\Users\Nick\workspace\CEG4166_RTS_Lib\Release" -mmcu=atmega2560 -o "EmptyProject.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

EmptyProject.lss: EmptyProject.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S EmptyProject.elf  >"EmptyProject.lss"
	@echo 'Finished building: $@'
	@echo ' '

EmptyProject.hex: EmptyProject.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex EmptyProject.elf  "EmptyProject.hex"
	@echo 'Finished building: $@'
	@echo ' '

EmptyProject.eep: EmptyProject.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex EmptyProject.elf  "EmptyProject.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: EmptyProject.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega2560 EmptyProject.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) EmptyProject.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Nick/workspace/FreeRTOS801/Release/libFreeRTOS801.a:
C:/Users/Nick/workspace/CEG4166_RTS_Lib/Release/libCEG4166_RTS_Lib.a:

-include ../makefile.targets
