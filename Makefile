# @file    Makefile
# @author  Pratik Dhulubulu
# @brief   Makefile.

# Toolchain
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
OBJDUMP  = arm-none-eabi-objdump
SIZE     = arm-none-eabi-size
GDB      = arm-none-eabi-gdb
RM       = rm -rf

# MCU Config
MCU       = cortex-m4
FPU       = fpv4-sp-d16
FLOAT_ABI = hard

ARCH_FLAGS = -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI)

# Project Folder Structure
APP_INIT      = App_Init
APP_TASKS     = App_Tasks
UART_HANDLER  = UART_Handler
DRIVERS_DIR   = Drivers
CORE_DIR      = Core
FREERTOS_DIR  = FreeRTOS_Kernel
LINKER_SCRIPT = Linker/linker_script.ld
BUILD_DIR     = Build

# Build Information
VERSION     = MY_RELEASE_1.0.0
BUILD_DATE := $(shell date +"%Y-%m-%d")
BUILD_TIME := $(shell date +"%H:%M:%S")

DEFS = \
-DBUILD_VERSION=\"$(VERSION)\" \
-DBUILD_DATE=\"$(BUILD_DATE)\" \
-DBUILD_TIME=\"$(BUILD_TIME)\"

# Build Mode
ifeq ($(RELEASE),1)
OPT = -O2
BUILD_TYPE = Release
else
OPT = -O0 -g
BUILD_TYPE = Debug
endif

# Source Directories
APP_SRCS = \
$(wildcard $(APP_INIT)/*.c) \
$(wildcard $(APP_TASKS)/*.c) \
$(wildcard $(UART_HANDLER)/*.c)

DRIVER_SRCS = \
$(foreach d,$(wildcard $(DRIVERS_DIR)/*),$(wildcard $(d)/*.c))

CORE_SRCS = \
$(wildcard $(CORE_DIR)/*/*.c)

FREERTOS_SRCS = \
$(FREERTOS_DIR)/tasks.c \
$(FREERTOS_DIR)/queue.c \
$(FREERTOS_DIR)/list.c \
$(FREERTOS_DIR)/portable/GCC/ARM_CM4F/port.c \
$(FREERTOS_DIR)/portable/MemMang/heap_4.c \
$(FREERTOS_DIR)/freertos_hooks.c

STARTUP_SRC = $(CORE_DIR)/Startup/startup_stm32f446.s

SRCS = \
$(APP_SRCS) \
$(DRIVER_SRCS) \
$(CORE_SRCS) \
$(FREERTOS_SRCS)

# Object Directories
OBJS = \
$(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS)) \
$(patsubst %.s,$(BUILD_DIR)/%.o,$(STARTUP_SRC))

# Include Directories
INCLUDES = \
-I$(APP_INIT) \
-I$(APP_TASKS) \
-I$(UART_HANDLER) \
-I$(CORE_DIR)/System \
-I$(CORE_DIR)/ISR \
-I$(FREERTOS_DIR) \
-I$(FREERTOS_DIR)/include \
-I$(FREERTOS_DIR)/portable/GCC/ARM_CM4F \
$(foreach d,$(wildcard $(DRIVERS_DIR)/*),-I$(d))

# Flags
NO_UNWIND_FLAGS = -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables
CFLAGS  = $(ARCH_FLAGS) $(OPT) -Wall -std=gnu11 $(DEFS) $(INCLUDES) $(NO_UNWIND_FLAGS)
ASFLAGS = $(ARCH_FLAGS) $(NO_UNWIND_FLAGS)
LDFLAGS = $(ARCH_FLAGS) -T$(LINKER_SCRIPT) -nostartfiles $(NO_UNWIND_FLAGS)

TARGET = firmware

# Targets
.PHONY: all clean flash info

all: $(BUILD_DIR)/$(TARGET).elf \
     $(BUILD_DIR)/$(TARGET).bin \
     $(BUILD_DIR)/$(TARGET).hex
	@echo "------------------------------------------"
	@echo " Build Type : $(BUILD_TYPE)"
	@echo " Version    : $(VERSION)"
	@echo " Date       : $(BUILD_DATE) $(BUILD_TIME)"
	@echo "------------------------------------------"

# Build Rules
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo "Assembling $<"
	@$(CC) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo "Linking..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@$(SIZE) $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@$(OBJCOPY) -O ihex $< $@

# Flashing
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	-c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

# Debugging with GDB and OpenOCD
debug: all
	@echo "Starting OpenOCD..."
	@nohup openocd -f interface/stlink.cfg \
	               -c "transport select swd" \
	               -f target/stm32f4x.cfg \
				   -d0 \
	               > $(BUILD_DIR)/openocd.log 2>&1 &
	@sleep 2
	@echo "Launching GDB..."
	$(GDB) $(BUILD_DIR)/$(TARGET).elf \
		-ex "set pagination off" \
		-ex "set confirm off" \
		-ex "target extended-remote localhost:3333" \
		-ex "monitor reset halt" \
		-ex "load" \
		-ex "monitor reset halt" \

kill_openocd:
	@pkill -f openocd

clean:
	@$(RM) $(BUILD_DIR)
	@echo "Clean done"

info:
	@echo "Sources:"
	@echo "$(SRCS)"
