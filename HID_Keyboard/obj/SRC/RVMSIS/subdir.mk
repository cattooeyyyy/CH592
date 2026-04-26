################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/RVMSIS/core_riscv.c 

C_DEPS += \
./SRC/RVMSIS/core_riscv.d 

OBJS += \
./SRC/RVMSIS/core_riscv.o 

DIR_OBJS += \
./SRC/RVMSIS/*.o \

DIR_DEPS += \
./SRC/RVMSIS/*.d \

DIR_EXPANDS += \
./SRC/RVMSIS/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
SRC/RVMSIS/%.o: ../SRC/RVMSIS/%.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Startup" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/APP/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HID_Keyboard/Profile/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/StdPeriphDriver/inc" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/HAL/include" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/Ld" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/BLE/LIB" -I"d:/Code/CH592/EVT(演示) - 副本/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

