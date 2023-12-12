################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LPS22HH/lps22hh_reg.c 

OBJS += \
./Drivers/LPS22HH/lps22hh_reg.o 

C_DEPS += \
./Drivers/LPS22HH/lps22hh_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LPS22HH/%.o Drivers/LPS22HH/%.su Drivers/LPS22HH/%.cyclo: ../Drivers/LPS22HH/%.c Drivers/LPS22HH/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Icones -I../Drivers/HTS221 -I../Drivers/LPS22HH -I../Drivers/BSP/STM32746G-Discovery -I../Utilities/Fonts -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-LPS22HH

clean-Drivers-2f-LPS22HH:
	-$(RM) ./Drivers/LPS22HH/lps22hh_reg.cyclo ./Drivers/LPS22HH/lps22hh_reg.d ./Drivers/LPS22HH/lps22hh_reg.o ./Drivers/LPS22HH/lps22hh_reg.su

.PHONY: clean-Drivers-2f-LPS22HH

