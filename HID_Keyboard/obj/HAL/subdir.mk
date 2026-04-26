################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
d:/Code/CH592/EVT(演示)\ -\ 副本/EXAM/BLE/HAL/MCU.c \
d:/Code/CH592/EVT(演示)\ -\ 副本/EXAM/BLE/HAL/RTC.c \
d:/Code/CH592/EVT(演示)\ -\ 副本/EXAM/BLE/HAL/SLEEP.c 

C_DEPS += \
./HAL/MCU.d \
./HAL/RTC.d \
./HAL/SLEEP.d 

OBJS += \
./HAL/MCU.o \
./HAL/RTC.o \
./HAL/SLEEP.o 

DIR_OBJS += \
./HAL/*.o \

DIR_DEPS += \
./HAL/*.d \

DIR_EXPANDS += \
./HAL/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
HAL/MCU.o: d:/Code/CH592/EVT(演示)\ -\ 副本/EXAM/BLE/HAL/MCU.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Startup" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/APP/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/Profile/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/StdPeriphDriver/inc" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HAL/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Ld" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/LIB" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
HAL/RTC.o: d:/Code/CH592/EVT(演示)\ -\ 副本/EXAM/BLE/HAL/RTC.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Startup" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/APP/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/Profile/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/StdPeriphDriver/inc" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HAL/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Ld" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/LIB" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
HAL/SLEEP.o: d:/Code/CH592/EVT(演示)\ -\ 副本/EXAM/BLE/HAL/SLEEP.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Startup" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/APP/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/Profile/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/StdPeriphDriver/inc" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HAL/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Ld" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/LIB" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

