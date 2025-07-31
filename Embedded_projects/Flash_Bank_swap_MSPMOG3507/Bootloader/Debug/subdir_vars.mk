################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../device_linker.cmd 

SYSCFG_SRCS += \
../Bootloader.syscfg 

C_SRCS += \
../Bootloader.c \
./ti_msp_dl_config.c \
C:/ti/mspm0_sdk_2_05_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c 

GEN_FILES += \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./Bootloader.d \
./ti_msp_dl_config.d \
./startup_mspm0g350x_ticlang.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./Bootloader.o \
./ti_msp_dl_config.o \
./startup_mspm0g350x_ticlang.o 

GEN_MISC_FILES += \
./device.cmd.genlibs \
./ti_msp_dl_config.h \
./Event.dot 

OBJS__QUOTED += \
"Bootloader.o" \
"ti_msp_dl_config.o" \
"startup_mspm0g350x_ticlang.o" 

GEN_MISC_FILES__QUOTED += \
"device.cmd.genlibs" \
"ti_msp_dl_config.h" \
"Event.dot" 

C_DEPS__QUOTED += \
"Bootloader.d" \
"ti_msp_dl_config.d" \
"startup_mspm0g350x_ticlang.d" 

GEN_FILES__QUOTED += \
"device.opt" \
"ti_msp_dl_config.c" 

C_SRCS__QUOTED += \
"../Bootloader.c" \
"./ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_2_05_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c" 

SYSCFG_SRCS__QUOTED += \
"../Bootloader.syscfg" 


