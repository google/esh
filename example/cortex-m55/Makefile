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

# should be v10 or above
TOOLCHAIN_PREFIX ?= arm-none-eabi-
OPTIMIZATION     = g

DEFINES = -mcpu=cortex-m55 -mthumb
ASM_FLAGS = -march=armv8-m.main

STARTUP = cortex-m

# MANDATORY:
#     These 3 parameters are needed for every port
#     Provide all values in hex
RAM_BASE_PHYSICAL  = 0x00000000
RAM_SIZE           = 0x4000
UART_BASE_PHYSICAL = 0x59303000
STACK_SIZE         = 0x1000

# setting the stack start manually for m class processors
DEFINES += -DSTACK_START=0x80000000UL

# MANDATORY:
#     Provide relative path to shell/ from current directory
#     Only Variable should be set above this line
SHELL_ROOT=../../shell
-include $(SHELL_ROOT)/Makefile
# ^^ Do not move the above line. All user targets to be defined below this line!

# ------------------------ User targets below this line ------------------------
run_debug:
	@qemu-system-arm -S -M mps3-an547 -cpu cortex-m55 -nographic -kernel \
	 $(PROJECT).elf -gdb tcp::1234

debug:
	@gdb-multiarch shell.elf -ex "target remote localhost:1234"

run: $(PROJECT).elf
	@qemu-system-arm -M mps3-an547 -cpu cortex-m55 -nographic \
	 -kernel $(PROJECT).elf
