################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_exti.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_flash.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_key.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_lcd.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_led.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_pit.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_pwm.c \
c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_system.c 

C_DEPS += \
./user_c/bsp/bsp_exti.d \
./user_c/bsp/bsp_flash.d \
./user_c/bsp/bsp_key.d \
./user_c/bsp/bsp_lcd.d \
./user_c/bsp/bsp_led.d \
./user_c/bsp/bsp_pit.d \
./user_c/bsp/bsp_pwm.d \
./user_c/bsp/bsp_system.d 

OBJS += \
./user_c/bsp/bsp_exti.o \
./user_c/bsp/bsp_flash.o \
./user_c/bsp/bsp_key.o \
./user_c/bsp/bsp_lcd.o \
./user_c/bsp/bsp_led.o \
./user_c/bsp/bsp_pit.o \
./user_c/bsp/bsp_pwm.o \
./user_c/bsp/bsp_system.o 

DIR_OBJS += \
./user_c/bsp/*.o \

DIR_DEPS += \
./user_c/bsp/*.d \

DIR_EXPANDS += \
./user_c/bsp/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
user_c/bsp/bsp_exti.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_exti.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/bsp/bsp_flash.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_flash.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/bsp/bsp_key.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_key.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/bsp/bsp_lcd.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_lcd.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/bsp/bsp_led.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_led.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/bsp/bsp_pit.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_pit.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/bsp/bsp_pwm.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_pwm.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/bsp/bsp_system.o: c:/Users/32656/Desktop/car/car/program/project/user/src/bsp/bsp_system.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car/car/program/project/code" -I"c:/Users/32656/Desktop/car/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

