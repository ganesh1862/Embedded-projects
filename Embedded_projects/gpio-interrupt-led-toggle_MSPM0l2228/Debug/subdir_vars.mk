################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../gpio-interrupt-led-toggle.syscfg 

C_SRCS += \
../gpio-interrupt-led-toggle.c \
./ti_msp_dl_config.c \
C:/ti/mspm0_sdk_2_05_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0l222x_ticlang.c 

GEN_CMDS += \
./device_linker.cmd 

GEN_FILES += \
./device_linker.cmd \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./gpio-interrupt-led-toggle.d \
./ti_msp_dl_config.d \
./startup_mspm0l222x_ticlang.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./gpio-interrupt-led-toggle.o \
./ti_msp_dl_config.o \
./startup_mspm0l222x_ticlang.o 

GEN_MISC_FILES += \
./device.cmd.genlibs \
./ti_msp_dl_config.h \
./Event.dot 

OBJS__QUOTED += \
"gpio-interrupt-led-toggle.o" \
"ti_msp_dl_config.o" \
"startup_mspm0l222x_ticlang.o" 

GEN_MISC_FILES__QUOTED += \
"device.cmd.genlibs" \
"ti_msp_dl_config.h" \
"Event.dot" 

C_DEPS__QUOTED += \
"gpio-interrupt-led-toggle.d" \
"ti_msp_dl_config.d" \
"startup_mspm0l222x_ticlang.d" 

GEN_FILES__QUOTED += \
"device_linker.cmd" \
"device.opt" \
"ti_msp_dl_config.c" 

C_SRCS__QUOTED += \
"../gpio-interrupt-led-toggle.c" \
"./ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_2_05_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0l222x_ticlang.c" 

SYSCFG_SRCS__QUOTED += \
"../gpio-interrupt-led-toggle.syscfg" 


