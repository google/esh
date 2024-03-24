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
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --jobs=$(shell nproc)

# Save the object files in this directory
BUILD_PATH = objects

# SHELL_ROOT is to be set my the user in their makefile and should point
# to the directory that current Makefile resides in (i.e shell/).
ifeq ($(SHELL_ROOT),)
    $(info Direct compilation from the shell/ is not permitted)
    $(info Please issue make command from the project directory)
    $(error )
endif

-include $(SHELL_ROOT)/build-magic/defaults.mk

# When set to 1, print dots for progress
ifeq ($(SILENT_BUILD),1)
PRINT_ONLY_ERRORS = 1> /dev/null
endif

# -------------------- Mandatory user configurable -----------------------------
# These variables are to be declared by the user in the project level Makefile
# The build system would use these as part of the automtion to generate a *.ld
# file, export required symbols and compile the files.

ifeq ($(USER_LAYOUT_FILE),)
LD_MACROS = --defsym __RAM_BASE__=$(RAM_BASE_PHYSICAL) --defsym __RAM_SIZE__=$(RAM_SIZE)
LD_FILE = $(shell realpath $(SHELL_ROOT)/scatter/one-seg.ld)

ifneq ($(ROM_BASE_PHYSICAL),)
LD_MACROS += --defsym __ROM_BASE__=$(ROM_BASE_PHYSICAL) --defsym __ROM_SIZE__=$(ROM_SIZE)
LD_FILE = $(shell realpath $(SHELL_ROOT)/scatter/two-seg.ld)
else
ROM_BASE_PHYSICAL = NA
ROM_SIZE = NA
endif

else
LD_FILE = $(shell realpath $(USER_LAYOUT_FILE))
RAM_BASE_PHYSICAL = NA
RAM_SIZE = NA
ROM_BASE_PHYSICAL = NA
ROM_SIZE = NA
endif

# -------------- defines to be passed to c code --------------------------------
# Every Physical Address should be defined as below
DEFINES += -DRAM_BASE_PHYSICAL=$(RAM_BASE_PHYSICAL)
DEFINES += -DECHO_INIT_VALUE=$(ECHO_INIT_VALUE)

# User defined prompt
PROMPT ?= \#

# note the use and host that the build was fired on
BUILD_USER=$(shell whoami)
BUILD_HOST=$(shell hostname)
SHELL_VERSION=$(shell cd $(SHELL_ROOT) && git rev-parse --short HEAD 2>/dev/null)
USER_REPO_VERSION=$(shell cd $(PROJECT_ROOT) && git rev-parse --short HEAD 2>/dev/null)

DEFINES += -D__PROMPT__=$(PROMPT)
DEFINES += -D__BUILD_USER__=$(BUILD_USER)
DEFINES += -D__BUILD_HOST__=$(BUILD_HOST)
DEFINES += -D__SHELL_VERSION__=$(SHELL_VERSION)
DEFINES += -D__USER_REPO_VERSION__=$(USER_REPO_VERSION)

# ------------------------------ feature control--------------------------------
# Exclude features if SHELL_LITE == 1
ifeq ($(SHELL_LITE),1)
DEFINES += -DSHELL_PRINTF_LITE
DEFINES += -DSHELL_NO_PRINTF_LL
DEFINES += -DSHELL_NO_UTILS
DEFINES += -DSHELL_NO_HISTORY
DEFINES += -DSHELL_NO_TAB_COMPLETE
DEFINES += -DSHELL_NO_BIT_UTILS
endif

# ------------------------------ toolchain -------------------------------------
GCC     = $(TOOLCHAIN_PREFIX)gcc
GPP     = $(TOOLCHAIN_PREFIX)g++
LD      = $(TOOLCHAIN_PREFIX)ld
AS      = $(TOOLCHAIN_PREFIX)gcc
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
BINSIZE = $(TOOLCHAIN_PREFIX)size

DEFAULT_LD_FLAGS = --print-memory-usage

# find all the *.c, *.cpp, *.S source files
-include $(SHELL_ROOT)/build-magic/find-source.mk

# process the include files
-include $(SHELL_ROOT)/build-magic/find-headers.mk

GCC_FLAGS = $(DEFINES) $(INCLUDES) -Wall -O$(OPTIMIZATION) -nostdlib
GCC_FLAGS += -nostartfiles -ffreestanding -ggdb -c

# ------------------------------ objects ---------------------------------------
OBJS ?=
OBJS += $(patsubst %.c,$(BUILD_PATH)/%.o,$(SRC_C))
OBJS += $(patsubst %.cpp,$(BUILD_PATH)/%.o,$(SRC_CPP))
OBJS += $(patsubst %.S,$(BUILD_PATH)/%.o,$(FILTERED_SRC_ASM))
OBJS += $(patsubst %.S,$(BUILD_PATH)/%.o,$(STARTUP_FILE))

DEP_HEADERS = $(H_FILES) $(HPP_FILES)

$(BUILD_PATH)/%.o:%.c $(DEP_HEADERS)
	@echo "compiling" $< $(PRINT_ONLY_ERRORS)
	@$(GCC) $(GCC_FLAGS) $< -o $@

$(BUILD_PATH)/%.o:%.cpp $(DEP_HEADERS)
	@echo "compiling" $< $(PRINT_ONLY_ERRORS)
	@$(GPP) $(GCC_FLAGS) -Wwrite-strings $< -o $@

$(BUILD_PATH)/%.o:%.S $(DEP_HEADERS)
	@echo "assembling" $< $(PRINT_ONLY_ERRORS)
	@$(AS) -c $(ASM_FLAGS)  $< -o $@

%.elf: $(OBJS) $(LD_FILE)
	@echo "generating" $@ $(PRINT_ONLY_ERRORS)
	@echo "Size Report: " $(PRINT_ONLY_ERRORS)
	@$(BINSIZE) -B -d -t $(OBJS) $(PRINT_ONLY_ERRORS)
	@$(LD) -T$(LD_FILE) $(OBJS) $(LD_FLAGS) $(DEFAULT_LD_FLAGS) $(LD_MACROS) \
	-o $@ -Map=$@.map $(PRINT_ONLY_ERRORS)
	@$(OBJDUMP) -D -S $@ > $@.lst

%.bin:%.elf
	@echo "generating" $@ $(PRINT_ONLY_ERRORS)
	@$(OBJCOPY) -O binary $< $@ $(PRINT_ONLY_ERRORS)
	@echo "Done!" $(PRINT_ONLY_ERRORS)

## default targetf
.DEFAULT_GOAL := all

### all: Builds the project binary. Same as just executing make.
all:: | config $(BUILD_PATH) $(PROJECT).bin
	@

$(BUILD_PATH):
	mkdir -p $(BUILD_PATH) $(shell dirname $(OBJS))

.PHONY: clean
### clean: Cleans the project
clean::
	@rm -rf $(OBJS) *.elf *.bin *.lst *.map *.gdb $(BUILD_PATH)

# helpful config and help targets
-include $(SHELL_ROOT)/build-magic/configuration.mk

# preserve all the object and the project elf file(s)
.PRECIOUS: $(OBJS) $(PROJECT).elf
VPATH = $(shell dirname $(SRC_C) $(SRC_CPP) $(FILTERED_SRC_ASM) $(STARTUP_FILE))
