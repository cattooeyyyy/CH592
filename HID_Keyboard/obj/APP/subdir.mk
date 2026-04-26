################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/app_led.c \
../APP/hidkbd.c \
../APP/hidkbd_main.c \
../APP/key_scan.c 

C_DEPS += \
./APP/app_led.d \
./APP/hidkbd.d \
./APP/hidkbd_main.d \
./APP/key_scan.d 

OBJS += \
./APP/app_led.o \
./APP/hidkbd.o \
./APP/hidkbd_main.o \
./APP/key_scan.o 

DIR_OBJS += \
./APP/*.o \

DIR_DEPS += \
./APP/*.d \

DIR_EXPANDS += \
./APP/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
APP/%.o: ../APP/%.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Startup" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/APP/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/Profile/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/StdPeriphDriver/inc" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HAL/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Ld" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/LIB" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

