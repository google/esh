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

# -------------------- Mandatory user configurable -----------------------------
# These variables are to be declared by the user in the project level Makefile
# The build system would use these as part of the automtion to generate a *.ld
# file, export required symbols and compile the files.

ifeq ($(TOOLCHAIN_PREFIX),)
    $(info TOOLCHAIN_PREFIX not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: TOOLCHAIN_PREFIX = arm-none-eabi- )
    $(error )
endif

ifeq ($(RAM_BASE_PHYSICAL),)
    $(info RAM_BASE_PHYSICAL not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: RAM_BASE_PHYSICAL = 0x00000000 )
    $(error )
endif

ifeq ($(RAM_SIZE),)
    $(info RAM_SIZE not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: RAM_SIZE = 0x4000 )
    $(error )
endif

ifeq ($(UART_BASE_PHYSICAL),)
    $(info UART_BASE_PHYSICAL not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: UART_BASE_PHYSICAL = 0x59303000 )
    $(error )
endif

ifeq ($(STARTUP),)
    $(info STARTUP not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: STARTUP = cortex-m.S )
    $(error )
endif

# Use these defaults if none provided by user
PROJECT            ?= shell
SHELL_ROOT         ?= ./
ECHO_INIT_VALUE    ?= 0x1

# -------------- defines to be passed to c code --------------------------------
# Every Physical Address should be defined as below
DEFINES += -DUART_BASE_PHYSICAL=$(UART_BASE_PHYSICAL)
DEFINES += -DRAM_BASE_PHYSICAL=$(RAM_BASE_PHYSICAL)
DEFINES += -DECHO_INIT_VALUE=$(ECHO_INIT_VALUE)

# note the use and host that the build was fired on
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

# ------------------------------ source ----------------------------------------
# discover the C source files in project and shell directory
SRC_C    += $(shell realpath $(shell find . $(SHELL_ROOT) \
			-type f -name "*.c" ) 2>/dev/null)

# discover the ASM source files in project directory
SRC_ASM  += $(shell realpath $(shell find . -type f -name "*.S") \
			2>/dev/null)

# search the startup file in bot project and shell directory
STARTUP_FILE ?= $(shell realpath $(shell find . $(SHELL_ROOT) \
				-type f -name "$(STARTUP)" -o  -name "$(STARTUP).S") \
				2>/dev/null)

# startup file may be included twice, filter it from the source
FILTERED_SRC_ASM = $(filter-out $(STARTUP_FILE), $(SRC_ASM))

# record all the paths where .h files are available
INCLUDES = $(shell find . $(SHELL_ROOT) -type f -name "*.h" | \
			sed -r "s|/[^/]+$$||" | \
			sort | uniq | \
			while read line; do echo "-I"$$line; done;)

# ------------------------------ optimization ----------------------------------
OPTIMIZATION ?= 0

GCC_FLAGS = $(DEFINES) $(INCLUDES) -Wall -O$(OPTIMIZATION) -nostdlib
GCC_FLAGS += -nostartfiles -ffreestanding -ggdb -c
LD_FLAGS  ?=

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
	sed 's/__RAM_SIZE__/$(RAM_SIZE)/g' > $@

%.o:%.c
	@echo "compiling" $^
	@$(GCC) $(GCC_FLAGS) $^ -o $@

%.o:%.S
	@echo "assembling" $^
	@$(AS) -c $(ASM_FLAGS)  $^ -o $@

%.elf: $(OBJS) $(LAYOUT_FILE)
	@echo "generating" $@
	@$(LD) -T$(LAYOUT_FILE) $(OBJS) $(LD_FLAGS) -o $@ -Map=$@.map
	@$(OBJDUMP) -D -S $@ > $@.lst

%.bin:%.elf
	@echo "section size: "
	@$(BINSIZE) -B $^
	@echo "generating" $@
	@$(OBJCOPY) -O binary $^ $@
	@echo "Done!"

all: $(PROJECT).bin $(PROJECT).elf
	@

.PHONY: clean
clean:
	@rm -rf $(LAYOUT_FILE) $(OBJS) *.elf *.bin *.lst *.map

# ------------------------------ configuration ---------------------------------
# Only printed when issued - make
ifeq ($(MAKECMDGOALS),)
$(info Detected Configuration)
$(info *)
$(info |- PROJECT      : $(PROJECT))
$(info |- BUILD USER   : $(BUILD_USER))
$(info |- BUILD HOST   : $(BUILD_HOST))
$(info |- SHELL_ROOT   : $(shell realpath $(SHELL_ROOT)))
$(info |- OPTIMIZATION : $(OPTIMIZATION))
$(info *)
$(info |- TOOLCHAIN    : $(TOOLCHAIN_PREFIX))
$(info |- STARTUP      : $(STARTUP_FILE))
$(info |- RAM BASE     : $(RAM_BASE_PHYSICAL))
$(info |- RAM SIZE     : $(RAM_SIZE))
$(info `- UART BASE    : $(UART_BASE_PHYSICAL))
$(info )
$(info >> If incorrect, please set these Variables in Makefile << )
$(info )
$(info Building...)
endif
