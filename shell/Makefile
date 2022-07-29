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

ifeq ($(USER_LAYOUT_FILE),)
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
else
RAM_BASE_PHYSICAL = NA
RAM_SIZE = NA
endif

ifeq ($(STARTUP),)
    $(info STARTUP not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: STARTUP = cortex-m.S )
    $(error )
endif

# Use these defaults if none provided by user
PROJECT            ?= shell
PROJECT_ROOT        = ./
SHELL_ROOT         ?= ./
DOCS                = $(SHELL_ROOT)/../docs/index.md
ECHO_INIT_VALUE    ?= 0x1
EXTERN_SRC         ?=
IGNORE_SRC_PATH    ?=

# -------------- defines to be passed to c code --------------------------------
# Every Physical Address should be defined as below
DEFINES += -DRAM_BASE_PHYSICAL=$(RAM_BASE_PHYSICAL)
DEFINES += -DECHO_INIT_VALUE=$(ECHO_INIT_VALUE)

# note the use and host that the build was fired on
BUILD_USER=$(shell whoami)
BUILD_HOST=$(shell hostname)
SHELL_VERSION=$(shell cd $(SHELL_ROOT) && git rev-parse --short HEAD 2>/dev/null)
USER_REPO_VERSION=$(shell cd $(PROJECT_ROOT) && git rev-parse --short HEAD 2>/dev/null)

DEFINES += -D__BUILD_USER__=$(BUILD_USER)
DEFINES += -D__BUILD_HOST__=$(BUILD_HOST)
DEFINES += -D__SHELL_VERSION__=$(SHELL_VERSION)
DEFINES += -D__USER_REPO_VERSION__=$(USER_REPO_VERSION)

# ------------------------------ feature control--------------------------------
# build with complete set of feature by default
SHELL_LITE ?= 0

# Exclude features if SHELL_LITE == 1
ifeq ($(SHELL_LITE),1)
DEFINES += -DSHELL_PRINTF_LITE
DEFINES += -DSHELL_NO_PRINTF_LL
DEFINES += -DSHELL_NO_UTILS
DEFINES += -DSHELL_NO_HISTORY
endif

# ------------------------------ toolchain -------------------------------------
GCC     = $(TOOLCHAIN_PREFIX)gcc
GPP     = $(TOOLCHAIN_PREFIX)g++
LD      = $(TOOLCHAIN_PREFIX)ld
AS      = $(TOOLCHAIN_PREFIX)gcc
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
BINSIZE = $(TOOLCHAIN_PREFIX)size

# ------------------------------ source ----------------------------------------
# discover the C source files in project and shell directory
ALL_SRC_C	+= $(shell realpath $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) \
				-type f -name "*.c" ) 2>/dev/null)
ALL_SRC_CPP	+= $(shell realpath $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) \
				-type f -name "*.cpp" ) 2>/dev/null)
ifneq ($(IGNORE_SRC_PATH),)
IGNORE_C	= $(shell realpath $(shell find $(IGNORE_SRC_PATH) \
				-type f -name "*.c" ) 2>/dev/null)
IGNORE_CPP	= $(shell realpath $(shell find $(IGNORE_SRC_PATH) \
				-type f -name "*.cpp" ) 2>/dev/null)
endif

SRC_C 	= $(filter-out $(IGNORE_C), $(ALL_SRC_C))
SRC_CPP = $(filter-out $(IGNORE_CPP), $(ALL_SRC_CPP))

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
ALL_H_FILES		+= $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) -type f -name "*.h")
ALL_HPP_FILES	+= $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) -type f -name "*.hpp")

ifneq ($(IGNORE_HEADER_PATH),)
IGNORE_H	= $(shell realpath $(shell find $(IGNORE_HEADER_PATH) \
				-type f -name "*.h" ) 2>/dev/null)
IGNORE_HPP	= $(shell realpath $(shell find $(IGNORE_HEADER_PATH) \
				-type f -name "*.hpp" ) 2>/dev/null)
endif

H_FILES		= $(filter-out $(IGNORE_H), $(ALL_H_FILES))
HPP_FILES	= $(filter-out $(IGNORE_HPP), $(ALL_HPP_FILES))

INCLUDES = $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) -type f -name "*.h" | \
			sed -r "s|/[^/]+$$||" | \
			sort | uniq | \
			while read line; do echo "-I"$$line; done;)
INCLUDES += $(shell find . $(SHELL_ROOT) -type f -name "*.hpp" | \
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
OBJS += $(patsubst %.cpp,%.o,$(SRC_CPP))
OBJS += $(patsubst %.S,%.o,$(FILTERED_SRC_ASM))
OBJS += $(patsubst %.S,%.o,$(STARTUP_FILE))

# ------------------------------ memory layout----------------------------------
# If the User is using a custom linker file then just assign the absolute path
# to LD_FILE and use that during the linking.
ifneq ($(USER_LAYOUT_FILE),)
LD_FILE = $(shell realpath $(USER_LAYOUT_FILE))
else
LAYOUT_FILE = $(shell realpath $(SHELL_ROOT)/scatter/layout.ld)
LD_FILE = $(LAYOUT_FILE)
LD_MACROS = --defsym __RAM_BASE__=$(RAM_BASE_PHYSICAL) --defsym __RAM_SIZE__=$(RAM_SIZE)
endif

%.o:%.c
	@echo "compiling" $^
	@$(GCC) $(GCC_FLAGS) $^ -o $@

%.o:%.cpp
	@echo "compiling" $^
	@$(GPP) $(GCC_FLAGS) -Wwrite-strings $^ -o $@

%.o:%.S
	@echo "assembling" $^
	@$(AS) -c $(ASM_FLAGS)  $^ -o $@

%.elf: $(OBJS) $(H_FILES) $(HPP_FILES) $(LD_FILE)
	@echo "generating" $@
	@$(LD) -T$(LD_FILE) $(OBJS) $(LD_FLAGS) $(LD_MACROS) -o $@ -Map=$@.map
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
	@rm -rf $(OBJS) *.elf *.bin *.lst *.map *.gdb

# ------------------------------ configuration ---------------------------------
# Only printed when issued - make
ifeq ($(MAKECMDGOALS),)
$(info Detected Configuration)
$(info *)
$(info ├── PROJECT             : $(PROJECT))
$(info ├── BUILD USER          : $(BUILD_USER))
$(info ├── BUILD HOST          : $(BUILD_HOST))
$(info ├── SHELL VERSION       : $(SHELL_VERSION))
$(info ├── USER REPO VERSION   : $(USER_REPO_VERSION))
$(info ├── SHELL ROOT          : $(shell realpath $(SHELL_ROOT)))
$(info ├── PROJECT ROOT        : $(shell realpath $(PROJECT_ROOT)))
$(info ├── OPTIMIZATION        : $(OPTIMIZATION))
$(info *)
$(info ├── TOOLCHAIN           : $(TOOLCHAIN_PREFIX))
$(info ├── STARTUP             : $(STARTUP_FILE))
$(info ├── LAYOUT FILE         : $(LD_FILE))
$(info ├── RAM BASE            : $(RAM_BASE_PHYSICAL))
$(info └── RAM SIZE            : $(RAM_SIZE))
$(info )
$(info >> If incorrect, please set these Variables in Makefile )
$(info >> Refer the documentation here: $(shell realpath $(DOCS)))
$(info )
endif
