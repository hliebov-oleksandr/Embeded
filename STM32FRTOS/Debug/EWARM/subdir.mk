################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../EWARM/startup_stm32f411xe.s 

S_DEPS += \
./EWARM/startup_stm32f411xe.d 

OBJS += \
./EWARM/startup_stm32f411xe.o 


# Each subdirectory must supply rules for building sources it contributes
EWARM/%.o: ../EWARM/%.s EWARM/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I/Users/alexxx/Embeded_/Embeded/STM32FRTOS/Middlewares/Third_Party/FreeRTOS/Source/include -I/Users/alexxx/Embeded_/Embeded/STM32FRTOS/Drivers/CMSIS/Include -I/Users/alexxx/Embeded_/Embeded/STM32FRTOS/Core/Inc -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-EWARM

clean-EWARM:
	-$(RM) ./EWARM/startup_stm32f411xe.d ./EWARM/startup_stm32f411xe.o

.PHONY: clean-EWARM

