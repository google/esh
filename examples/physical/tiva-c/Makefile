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

TOOLCHAIN_PREFIX ?= arm-none-eabi-
OPTIMIZATION = g

STARTUP = start

DEFINES = -mcpu=cortex-m4 -mthumb -mtune=cortex-m4
DEFINES += -DSHELL_NO_PRINTF_LL

ASM_FLAGS = -march=armv7e-m

ROM_BASE_PHYSICAL = 0x00000000
ROM_SIZE = 0x00040000

RAM_BASE_PHYSICAL = 0x20000000
RAM_SIZE = 0x00008000

# MANDATORY:
#     Provide relative path to shell/ from current directory
#     Only Variable should be set above this line
SHELL_ROOT=../../../shell
-include $(SHELL_ROOT)/Makefile
# ^^ Do not move the above line. All user targets to be defined below this line!

# ------------------------ User targets below this line ------------------------
ICDI = /dev/ttyACM0

# OpenOCD specific variables
OPENOC_CONFIG_FILE =/usr/local/share/openocd/scripts/board/ek-lm4f120xl.cfg
OPENOC_FILE_UPLOAD ="program $(PROJECT).elf verify reset exit"

# Uploads the .hex file to the board
upload: $(PROJECT).elf
	sudo openocd --file $(OPENOC_CONFIG_FILE) -c $(OPENOC_FILE_UPLOAD)

debug: $(PROJECT).elf upload
	@echo "target extended-remote localhost:3333" > $(PROJECT).gdb
	@echo "monitor reset halt" >> $(PROJECT).gdb
	@echo "GDB server active..."
	@echo "Use Ctrl+c to close the server!"
	@sudo openocd -f $(OPENOC_CONFIG_FILE) 2>/dev/null

gdb: $(PROJECT).elf
	@gdb-multiarch -q $^ --command=$(PROJECT).gdb
