#
# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

MAKEFLAGS += --silent

# ------------------------------ user configurable -----------------------------
# Use these defaults if none provided by user
PROJECT            ?= shell
SHELL_ROOT         ?= ./
STARTUP            ?= riscv64
ECHO_INIT_VALUE    ?= 0x1

# Memory and UART address. Use riscv64, qemu target defaults
RAM_BASE_PHYSICAL  ?= 0x80000000
RAM_SIZE           ?= 0x4000000
UART_BASE_PHYSICAL ?= 0x10000000
STACK_SIZE         ?= 0x1000

# Build for RISCV-64 by default
TOOLCHAIN_PREFIX   ?= riscv64-linux-gnu-

# ------------------------------ defines for C code ----------------------------
# Every Physical Address should be defined as below
DEFINES += -DUART_BASE_PHYSICAL=$(UART_BASE_PHYSICAL)
DEFINES += -DRAM_BASE_PHYSICAL=$(RAM_BASE_PHYSICAL)
DEFINES += -DECHO_INIT_VALUE=$(ECHO_INIT_VALUE)

BUILD_USER=$(shell whoami)
BUILD_HOST=$(shell hostname)
DEFINES += -D__BUILD_USER__=$(BUILD_USER)
DEFINES += -D__BUILD_HOST__=$(BUILD_HOST)

# ------------------------------ toolchain -------------------------------------
GCC     = $(TOOLCHAIN_PREFIX)gcc
LD      = $(TOOLCHAIN_PREFIX)ld
AS      = $(TOOLCHAIN_PREFIX)gcc
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
BINSIZE = $(TOOLCHAIN_PREFIX)size

# ------------------------------ optimization ----------------------------------
OPTIMIZATION ?= 0

FLAGS   = $(DEFINES) -I$(INC) -Wall -O$(OPTIMIZATION) -nostdlib
FLAGS  += -nostartfiles -ffreestanding -ggdb -c


LD_FLAGS ?=

# ------------------------------ source ----------------------------------------
INC    ?= $(shell realpath $(SHELL_ROOT)/include/)
SRC_C  += $(shell realpath $(shell find $(SHELL_ROOT) -type f -name "*.c"))

ifneq ($(SHELL_ROOT), ./)
SRC_C  += $(shell realpath $(shell find . -type f -name "*.c"))
ASM_F  += $(shell find . -type f -name "*.S")
ifneq ($(ASM_F),)
SRC_ASM = $(shell realpath $(ASM_F))
endif
endif

STARTUP_FILE ?= $(shell realpath $(SHELL_ROOT)/startup/$(STARTUP).S)
FILTERED_SRC_ASM = $(filter-out $(STARTUP_FILE), $(SRC_ASM))

# ------------------------------ objects ---------------------------------------
OBJS ?=
OBJS += $(patsubst %.c,%.o,$(SRC_C))
OBJS += $(patsubst %.S,%.o,$(FILTERED_SRC_ASM))
OBJS += $(patsubst %.S,%.o,$(STARTUP_FILE))

# ------------------------------ memory layout----------------------------------
LAYOUT_FILE = $(shell realpath $(SHELL_ROOT)/scatter/layout.ld)

# ------------------------------ targets ---------------------------------------
%.ld: %.ldt
	@echo "generating" $@
	@cp $^ $@
	@sed 's/__RAM_BASE__/$(RAM_BASE_PHYSICAL)/g' $^ | \
	sed 's/__RAM_SIZE__/$(RAM_SIZE)/g' | \
	sed 's/__STACK_SIZE__/$(STACK_SIZE)/g' > $@

%.o:%.c
	@echo "compiling" $^
	@$(GCC) $(FLAGS) $^ -o $@

%.o:%.S
	@echo "assembling" $^
	$(AS) -c $(ASM_FLAGS)  $^ -o $@

%.elf: $(OBJS) $(LAYOUT_FILE)
	@echo "generating" $@
	@$(LD) -T$(LAYOUT_FILE) $(OBJS) $(LD_FLAGS) -o $@
	@$(OBJDUMP) -D -S $@ > $(PROJECT).lst

%.bin:%.elf
	@echo "section size: "
	@$(BINSIZE) -B $^
	@echo "generating" $@
	@$(OBJCOPY) -O binary $^ $@
	@echo "Done!"

all: $(PROJECT).bin
	@

.PHONY: clean
clean:
	@rm -rf $(LAYOUT_FILE) $(OBJS) $(PROJECT).elf $(PROJECT).bin $(PROJECT).lst

# ------------------------------ configuration ---------------------------------
# Only printed when issued - make
ifeq ($(MAKECMDGOALS),)
$(info Detected Configuration)
$(info *)
$(info |- PROJECT      : $(PROJECT))
$(info |- BUILD USER   : $(BUILD_USER))
$(info |- BUILD HOST   : $(BUILD_HOST))
$(info |- STARTUP      : $(STARTUP).S)
$(info |- TOOLCHAIN    : $(TOOLCHAIN_PREFIX))
$(info |- OPTIMIZATION : $(OPTIMIZATION))
$(info *)
$(info |- RAM BASE     : $(RAM_BASE_PHYSICAL))
$(info |- RAM SIZE     : $(RAM_SIZE))
$(info |- UART BASE    : $(UART_BASE_PHYSICAL))
$(info `- STACK SIZE   : $(STACK_SIZE))
$(info )
$(info >> If incorrect, please set these Variables in Makefile << )
$(info )
$(info Building...)
endif
