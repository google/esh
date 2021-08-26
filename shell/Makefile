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

# ------------------------------ toolchain -------------------------------------
GCC     = $(TOOLCHAIN_PREFIX)gcc
LD      = $(TOOLCHAIN_PREFIX)ld
AS      = $(TOOLCHAIN_PREFIX)as
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
BINSIZE = $(TOOLCHAIN_PREFIX)size

FLAGS   = $(DEFINES) -O0 -I$(INC) -Wall -O0 -nostdlib
FLAGS  += -nostartfiles -ffreestanding -ggdb -c

# ------------------------------ source ----------------------------------------
INC    ?= $(shell realpath $(SHELL_ROOT)/include/)
SRC_C  += $(shell realpath $(shell find $(SHELL_ROOT) -iname "*.c"))

ifneq ($(SHELL_ROOT), ./)
SRC_C  += $(shell realpath $(shell find -iname "*.c"))
ASM_F  += $(shell find -iname "*.S")
ifneq ($(ASM_F),)
SRC_ASM = $(shell realpath $(ASM_F))
endif
endif

STARTUP_FILE += $(shell realpath $(SHELL_ROOT)/startup/$(STARTUP).S)

# ------------------------------ objects ---------------------------------------
OBJS  = $(patsubst %.c,%.o,$(SRC_C))
OBJS += $(patsubst %.S,%.o,$(SRC_ASM))
OBJS += $(patsubst %.S,%.o,$(STARTUP_FILE))

# ------------------------------ memory layout----------------------------------
LAYOUT_FILE = $(shell realpath $(SHELL_ROOT)/layout.ld)

# ------------------------------ targets ---------------------------------------
%.ld: %.ldt
	@echo "generating" $@
	@cp $^ $@
	@sed -i 's/__RAM_BASE__/$(RAM_BASE_PHYSICAL)/g' $@
	@sed -i 's/__RAM_SIZE__/$(RAM_SIZE)/g' $@
	@sed -i 's/__STACK_SIZE__/$(STACK_SIZE)/g' $@

%.o:%.c
	@echo "compiling" $^
	@$(GCC) $(FLAGS) $^ -o $@

%.o:%.S
	@echo "assembling" $^
	@$(AS) -c $^ -o $@

%.elf: $(OBJS) $(LAYOUT_FILE)
	@echo "generating" $@
	@$(LD) -T$(LAYOUT_FILE) $(OBJS) -o $@
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
$(info |- PROJECT    : $(PROJECT))
$(info |- STARTUP    : $(STARTUP).S)
$(info |- TOOLCHAIN  : $(TOOLCHAIN_PREFIX))
$(info *)
$(info |- RAM BASE   : $(RAM_BASE_PHYSICAL))
$(info |- RAM SIZE   : $(RAM_SIZE))
$(info |- UART BASE  : $(UART_BASE_PHYSICAL))
$(info `- STACK SIZE : $(STACK_SIZE))
$(info )
$(info >> If incorrect, please set these Variables in Makefile << )
$(info )
$(info Building...)
endif
