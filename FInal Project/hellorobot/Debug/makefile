################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
hellorobot.elf.size \

ELFCHECK += \
hellorobot.elf.elfcheck \


# All Target
all: hellorobot.elf secondary-outputs

# Tool invocations
hellorobot.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MicroBlaze g++ linker'
	mb-g++ -Wl,-T -Wl,../src/lscript.ld -L../../hellorobot_bsp/microblaze_0/lib -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.50.b -mno-xl-soft-mul -Wl,--no-relax -Wl,--gc-sections -o "hellorobot.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

hellorobot.elf.size: hellorobot.elf
	@echo 'Invoking: MicroBlaze Print Size'
	mb-size hellorobot.elf  |tee "hellorobot.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

hellorobot.elf.elfcheck: hellorobot.elf
	@echo 'Invoking: Xilinx ELF Check'
	elfcheck hellorobot.elf -hw ../../FinalJoe_hw_platform/system.xml -pe microblaze_0  |tee "hellorobot.elf.elfcheck"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(ELFSIZE)$(CXX_DEPS)$(C_UPPER_DEPS)$(ELFCHECK)$(S_UPPER_DEPS) hellorobot.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
