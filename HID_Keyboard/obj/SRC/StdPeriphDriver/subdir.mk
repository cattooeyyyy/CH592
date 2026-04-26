################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/StdPeriphDriver/CH59x_adc.c \
../SRC/StdPeriphDriver/CH59x_clk.c \
../SRC/StdPeriphDriver/CH59x_flash.c \
../SRC/StdPeriphDriver/CH59x_gpio.c \
../SRC/StdPeriphDriver/CH59x_i2c.c \
../SRC/StdPeriphDriver/CH59x_lcd.c \
../SRC/StdPeriphDriver/CH59x_pwm.c \
../SRC/StdPeriphDriver/CH59x_pwr.c \
../SRC/StdPeriphDriver/CH59x_spi0.c \
../SRC/StdPeriphDriver/CH59x_sys.c \
../SRC/StdPeriphDriver/CH59x_timer0.c \
../SRC/StdPeriphDriver/CH59x_timer1.c \
../SRC/StdPeriphDriver/CH59x_timer2.c \
../SRC/StdPeriphDriver/CH59x_timer3.c \
../SRC/StdPeriphDriver/CH59x_uart0.c \
../SRC/StdPeriphDriver/CH59x_uart1.c \
../SRC/StdPeriphDriver/CH59x_uart2.c \
../SRC/StdPeriphDriver/CH59x_uart3.c \
../SRC/StdPeriphDriver/CH59x_usbdev.c \
../SRC/StdPeriphDriver/CH59x_usbhostBase.c \
../SRC/StdPeriphDriver/CH59x_usbhostClass.c 

C_DEPS += \
./SRC/StdPeriphDriver/CH59x_adc.d \
./SRC/StdPeriphDriver/CH59x_clk.d \
./SRC/StdPeriphDriver/CH59x_flash.d \
./SRC/StdPeriphDriver/CH59x_gpio.d \
./SRC/StdPeriphDriver/CH59x_i2c.d \
./SRC/StdPeriphDriver/CH59x_lcd.d \
./SRC/StdPeriphDriver/CH59x_pwm.d \
./SRC/StdPeriphDriver/CH59x_pwr.d \
./SRC/StdPeriphDriver/CH59x_spi0.d \
./SRC/StdPeriphDriver/CH59x_sys.d \
./SRC/StdPeriphDriver/CH59x_timer0.d \
./SRC/StdPeriphDriver/CH59x_timer1.d \
./SRC/StdPeriphDriver/CH59x_timer2.d \
./SRC/StdPeriphDriver/CH59x_timer3.d \
./SRC/StdPeriphDriver/CH59x_uart0.d \
./SRC/StdPeriphDriver/CH59x_uart1.d \
./SRC/StdPeriphDriver/CH59x_uart2.d \
./SRC/StdPeriphDriver/CH59x_uart3.d \
./SRC/StdPeriphDriver/CH59x_usbdev.d \
./SRC/StdPeriphDriver/CH59x_usbhostBase.d \
./SRC/StdPeriphDriver/CH59x_usbhostClass.d 

OBJS += \
./SRC/StdPeriphDriver/CH59x_adc.o \
./SRC/StdPeriphDriver/CH59x_clk.o \
./SRC/StdPeriphDriver/CH59x_flash.o \
./SRC/StdPeriphDriver/CH59x_gpio.o \
./SRC/StdPeriphDriver/CH59x_i2c.o \
./SRC/StdPeriphDriver/CH59x_lcd.o \
./SRC/StdPeriphDriver/CH59x_pwm.o \
./SRC/StdPeriphDriver/CH59x_pwr.o \
./SRC/StdPeriphDriver/CH59x_spi0.o \
./SRC/StdPeriphDriver/CH59x_sys.o \
./SRC/StdPeriphDriver/CH59x_timer0.o \
./SRC/StdPeriphDriver/CH59x_timer1.o \
./SRC/StdPeriphDriver/CH59x_timer2.o \
./SRC/StdPeriphDriver/CH59x_timer3.o \
./SRC/StdPeriphDriver/CH59x_uart0.o \
./SRC/StdPeriphDriver/CH59x_uart1.o \
./SRC/StdPeriphDriver/CH59x_uart2.o \
./SRC/StdPeriphDriver/CH59x_uart3.o \
./SRC/StdPeriphDriver/CH59x_usbdev.o \
./SRC/StdPeriphDriver/CH59x_usbhostBase.o \
./SRC/StdPeriphDriver/CH59x_usbhostClass.o 

DIR_OBJS += \
./SRC/StdPeriphDriver/*.o \

DIR_DEPS += \
./SRC/StdPeriphDriver/*.d \

DIR_EXPANDS += \
./SRC/StdPeriphDriver/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
SRC/StdPeriphDriver/%.o: ../SRC/StdPeriphDriver/%.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Startup" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/APP/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/Profile/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/StdPeriphDriver/inc" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HAL/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Ld" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/LIB" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

