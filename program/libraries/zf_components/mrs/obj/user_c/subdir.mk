################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/32656/Desktop/car/car/program/project/user/src/isr.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/main.c 

C_DEPS += \
./user_c/isr.d \
./user_c/main.d 

OBJS += \
./user_c/isr.o \
./user_c/main.o 

DIR_OBJS += \
./user_c/*.o \

DIR_DEPS += \
./user_c/*.d \

DIR_EXPANDS += \
./user_c/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
user_c/isr.o: c:/Users/32656/Desktop/car/car/program/project/user/src/isr.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/main.o: c:/Users/32656/Desktop/car/car/program/project/user/src/main.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

