################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.o Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.su Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.cyclo: ../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.c Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F401C_DISCO -DSTM32F4 -DSTM32F401VCTx -c -I../Inc -I/Users/alexxx/Embeded_/Embeded/STM32FRTOS/Drivers/CMSIS/Include -I/Users/alexxx/Embeded_/Embeded/STM32FRTOS/Middlewares/Third_Party/FreeRTOS/Source/include -I/Users/alexxx/Embeded_/Embeded/STM32FRTOS/Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-MemMang

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-MemMang:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d ./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o ./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-MemMang

