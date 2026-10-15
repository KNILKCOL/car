################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_main.c \
c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_menu.c \
c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_param.c \
c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_task.c \
c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_ui.c 

C_DEPS += \
./user_c/app/app_main.d \
./user_c/app/app_menu.d \
./user_c/app/app_param.d \
./user_c/app/app_task.d \
./user_c/app/app_ui.d 

OBJS += \
./user_c/app/app_main.o \
./user_c/app/app_menu.o \
./user_c/app/app_param.o \
./user_c/app/app_task.o \
./user_c/app/app_ui.o 

DIR_OBJS += \
./user_c/app/*.o \

DIR_DEPS += \
./user_c/app/*.d \

DIR_EXPANDS += \
./user_c/app/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
user_c/app/app_main.o: c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_main.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car(1)/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car(1)/car/program/project/code" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/app/app_menu.o: c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_menu.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car(1)/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car(1)/car/program/project/code" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/app/app_param.o: c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_param.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car(1)/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car(1)/car/program/project/code" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/app/app_task.o: c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_task.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car(1)/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car(1)/car/program/project/code" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
user_c/app/app_ui.o: c:/Users/32656/Desktop/car(1)/car/program/project/user/src/app/app_ui.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"c:/Users/32656/Desktop/car(1)/car/program/Libraries/doc" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_components" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Core" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Ld" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Peripheral" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/sdk/Startup" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/app" -I"c:/Users/32656/Desktop/car(1)/car/program/project/user/inc/bsp" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_common" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_device" -I"c:/Users/32656/Desktop/car(1)/car/program/project/code" -I"c:/Users/32656/Desktop/car(1)/car/program/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

