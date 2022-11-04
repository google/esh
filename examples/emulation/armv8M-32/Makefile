#
# Copyright 2022 Google LLC
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

# NOTE: gcc-arm should be v10.3.1 or above
TOOLCHAIN_PATH ?=
TOOLCHAIN_PREFIX ?= arm-none-eabi-
OPTIMIZATION     = g

TOOLCHAIN_VERSION := $(shell $(TOOLCHAIN_PREFIX)gcc -dumpversion)
ifneq ($(TOOLCHAIN_VERSION), 10.3.1)
dummy:
	echo -e "\033[0;31mIncorrect toolchain version, need 10.3.1 \nBuild Failed!\033[0m"
	exit
endif

LIB_GCC_PATH = /usr/lib/gcc/arm-none-eabi/$(TOOLCHAIN_VERSION)/thumb/v8-m.main+fp/softfp

DEFINES = -mcpu=cortex-m33 -mthumb
ASM_FLAGS = -march=armv8-m.main
LD_FLAGS = -lgcc -L$(LIB_GCC_PATH)

STARTUP = cortex-m33

ROM_BASE_PHYSICAL = 0x10000000
ROM_SIZE          = 0x0007FFFF
RAM_BASE_PHYSICAL = 0x30000000
RAM_SIZE          = 0x00007FFF

DEFINES += -DSHELL_NO_PRINTF_LL
DEFINES += -DSTACK_START=0x30007FFF #32KB Stack in SRAM0 for core0
DEFINES += -DSTACK_START2=0x3000FFFF #32KB stack in SRAM1 for core1

# MANDATORY:
#     Provide relative path to shell/ from current directory
#     Only Variable should be set above this line
SHELL_ROOT=../../../shell
-include $(SHELL_ROOT)/Makefile
# ^^ Do not move the above line. All user targets to be defined below this line!

# ------------------------ User targets below this line ------------------------
### run: Launches QEMU and loads the project binary on it
run: $(PROJECT).elf
	@echo "Use Ctrl+A X to exit Qemu"
	@qemu-system-arm \
		-M mps3-an524 \
		-cpu cortex-m33 \
		-smp 2 \
		-nographic \
		-kernel $(PROJECT).elf

### debug: Qemu with GDB server. To be executed before 'make gdb'
debug:
	@echo Please use \"make gdb\" in another terminal to attach gdb.
	@echo Use \"cltr+a,x\" to quit qemu debug
	@qemu-system-arm -S \
		-M mps3-an524 \
		-cpu cortex-m33 \
		-smp 2 \
		-nographic \
		-kernel $(PROJECT).elf \
		-gdb tcp::4444

### gdb: Launches GDB and connects to qemu. To be executed after 'make debug'
gdb:
	@gdb-multiarch $(PROJECT).elf -ex "target extended-remote localhost:4444"
