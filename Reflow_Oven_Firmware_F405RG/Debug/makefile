################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
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
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Reflow_Oven_Firmware_F405RG.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Reflow_Oven_Firmware_F405RG.list \

OBJCOPY_BIN += \
Reflow_Oven_Firmware_F405RG.bin \


# All Target
all: Reflow_Oven_Firmware_F405RG.elf secondary-outputs

# Tool invocations
Reflow_Oven_Firmware_F405RG.elf: $(OBJS) $(USER_OBJS) /home/jh/STM32Cube/Temperierofen/Reflow_Oven_Firmware_F405RG/STM32F405RGTX_FLASH.ld
	arm-none-eabi-g++ -o "Reflow_Oven_Firmware_F405RG.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/jh/STM32Cube/Temperierofen/Reflow_Oven_Firmware_F405RG/STM32F405RGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Reflow_Oven_Firmware_F405RG.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Reflow_Oven_Firmware_F405RG.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Reflow_Oven_Firmware_F405RG.list"
	@echo 'Finished building: $@'
	@echo ' '

Reflow_Oven_Firmware_F405RG.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Reflow_Oven_Firmware_F405RG.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
