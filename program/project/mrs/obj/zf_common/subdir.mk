################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_clock.c \
c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_debug.c \
c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_fifo.c \
c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_font.c \
c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_function.c \
c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_interrupt.c 

C_DEPS += \
./zf_common/zf_common_clock.d \
./zf_common/zf_common_debug.d \
./zf_common/zf_common_fifo.d \
./zf_common/zf_common_font.d \
./zf_common/zf_common_function.d \
./zf_common/zf_common_interrupt.d 

OBJS += \
./zf_common/zf_common_clock.o \
./zf_common/zf_common_debug.o \
./zf_common/zf_common_fifo.o \
./zf_common/zf_common_font.o \
./zf_common/zf_common_function.o \
./zf_common/zf_common_interrupt.o 

DIR_OBJS += \
./zf_common/*.o \

DIR_DEPS += \
./zf_common/*.d \

DIR_EXPANDS += \
./zf_common/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
zf_common/zf_common_clock.o: c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_clock.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_debug.o: c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_debug.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_fifo.o: c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_fifo.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_font.o: c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_font.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_function.o: c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_function.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_interrupt.o: c:/Users/32656/Desktop/car/car/program/libraries/zf_common/zf_common_interrupt.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

