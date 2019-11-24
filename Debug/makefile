################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Vending-Machine-As-Time-Machine-With-SEOS.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Vending-Machine-As-Time-Machine-With-SEOS.list \


# All Target
all: Vending-Machine-As-Time-Machine-With-SEOS.elf secondary-outputs

# Tool invocations
Vending-Machine-As-Time-Machine-With-SEOS.elf: $(OBJS) $(USER_OBJS) D:\Data\ YOGI\Kuliah\Semester\ 5\Sistem\ Embedded\SisEmb\Vending-Machine-As-Time-Machine-With-SEOS\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "Vending-Machine-As-Time-Machine-With-SEOS.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\Data YOGI\Kuliah\Semester 5\Sistem Embedded\SisEmb\Vending-Machine-As-Time-Machine-With-SEOS\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Vending-Machine-As-Time-Machine-With-SEOS.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Vending-Machine-As-Time-Machine-With-SEOS.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Vending-Machine-As-Time-Machine-With-SEOS.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
