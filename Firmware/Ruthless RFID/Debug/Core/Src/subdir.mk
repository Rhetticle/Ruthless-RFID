################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MFRC.c \
../Core/Src/OLED.c \
../Core/Src/W25N01GVZEIG.c \
../Core/Src/button.c \
../Core/Src/commands.c \
../Core/Src/freertos.c \
../Core/Src/main.c \
../Core/Src/ruthlessfs.c \
../Core/Src/screen.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/terminal.c 

OBJS += \
./Core/Src/MFRC.o \
./Core/Src/OLED.o \
./Core/Src/W25N01GVZEIG.o \
./Core/Src/button.o \
./Core/Src/commands.o \
./Core/Src/freertos.o \
./Core/Src/main.o \
./Core/Src/ruthlessfs.o \
./Core/Src/screen.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/terminal.o 

C_DEPS += \
./Core/Src/MFRC.d \
./Core/Src/OLED.d \
./Core/Src/W25N01GVZEIG.d \
./Core/Src/button.d \
./Core/Src/commands.d \
./Core/Src/freertos.d \
./Core/Src/main.d \
./Core/Src/ruthlessfs.d \
./Core/Src/screen.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/terminal.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/MFRC.cyclo ./Core/Src/MFRC.d ./Core/Src/MFRC.o ./Core/Src/MFRC.su ./Core/Src/OLED.cyclo ./Core/Src/OLED.d ./Core/Src/OLED.o ./Core/Src/OLED.su ./Core/Src/W25N01GVZEIG.cyclo ./Core/Src/W25N01GVZEIG.d ./Core/Src/W25N01GVZEIG.o ./Core/Src/W25N01GVZEIG.su ./Core/Src/button.cyclo ./Core/Src/button.d ./Core/Src/button.o ./Core/Src/button.su ./Core/Src/commands.cyclo ./Core/Src/commands.d ./Core/Src/commands.o ./Core/Src/commands.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/ruthlessfs.cyclo ./Core/Src/ruthlessfs.d ./Core/Src/ruthlessfs.o ./Core/Src/ruthlessfs.su ./Core/Src/screen.cyclo ./Core/Src/screen.d ./Core/Src/screen.o ./Core/Src/screen.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/terminal.cyclo ./Core/Src/terminal.d ./Core/Src/terminal.o ./Core/Src/terminal.su

.PHONY: clean-Core-2f-Src

