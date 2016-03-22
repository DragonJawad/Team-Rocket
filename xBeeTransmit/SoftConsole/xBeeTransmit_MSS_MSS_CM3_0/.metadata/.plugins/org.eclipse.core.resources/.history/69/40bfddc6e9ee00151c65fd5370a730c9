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
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
MEMORYMAP += \
memory-map.xml \

IHEXFILES += \
xBeeTransmit_MSS_MSS_CM3_0_app.hex \

SRECFILES += \
xBeeTransmit_MSS_MSS_CM3_0_app.srec \

LISTINGS += \
xBeeTransmit_MSS_MSS_CM3_0_app.lst \


# All Target
all: xBeeTransmit_MSS_MSS_CM3_0_app secondary-outputs

# Tool invocations
xBeeTransmit_MSS_MSS_CM3_0_app: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU C Linker'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -L"C:\Users\Ben\Desktop\xBeeTransmit\SoftConsole\xBeeTransmit_MSS_MSS_CM3_0\xBeeTransmit_MSS_MSS_CM3_0_hw_platform\Debug" -T../../xBeeTransmit_MSS_MSS_CM3_0_hw_platform/CMSIS/startup_gcc/debug-in-actel-smartfusion-esram.ld -Wl,-Map=$(EXECUTABLE).map -Xlinker -gc-sections -o"xBeeTransmit_MSS_MSS_CM3_0_app" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

memory-map.xml: $(EXECUTABLE)
	@echo 'Invoking: Memory map generator'
	actel-map -o "memory-map.xml" $(EXECUTABLE) $(BUILDCMD)
	@echo 'Finished building: $@'
	@echo ' '

xBeeTransmit_MSS_MSS_CM3_0_app.hex: $(EXECUTABLE)
	@echo 'Invoking: GNU Intel Hex File Generator'
	arm-none-eabi-objcopy -O ihex $(EXECUTABLE) "xBeeTransmit_MSS_MSS_CM3_0_app.hex"
	@echo 'Finished building: $@'
	@echo ' '

xBeeTransmit_MSS_MSS_CM3_0_app.srec: $(EXECUTABLE)
	@echo 'Invoking: GNU S-Record Generator'
	arm-none-eabi-objcopy -O srec $(EXECUTABLE) "xBeeTransmit_MSS_MSS_CM3_0_app.srec"
	@echo 'Finished building: $@'
	@echo ' '

xBeeTransmit_MSS_MSS_CM3_0_app.lst: $(EXECUTABLE)
	@echo 'Invoking: GNU Listing Generator'
	arm-none-eabi-objdump -h -S $(EXECUTABLE) > "xBeeTransmit_MSS_MSS_CM3_0_app.lst"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean::
	-$(RM) $(MEMORYMAP)$(OBJS)$(C_DEPS)$(SRECFILES)$(IHEXFILES)$(LISTINGS)$(EXECUTABLE) xBeeTransmit_MSS_MSS_CM3_0_app
	-@echo ' '

secondary-outputs: $(MEMORYMAP) $(IHEXFILES) $(SRECFILES) $(LISTINGS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
