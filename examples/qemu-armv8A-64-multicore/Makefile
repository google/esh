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

TOOLCHAIN_PREFIX = aarch64-linux-gnu-
OPTIMIZATION     = g
STARTUP          = aarch64-multicore

# MANDATORY:
#     These 3 parameters are needed for every port
#     Provide all values in hex
RAM_BASE_PHYSICAL  = 0x40100000
RAM_SIZE           = 0x40000

# MANDATORY:
#     Provide relative path to shell/ from current directory
#     Only Variable should be set above this line
SHELL_ROOT=../../shell
-include $(SHELL_ROOT)/Makefile
# ^^ Do not move the above line. All user targets to be defined below this line!

# ------------------------ User targets below this line ------------------------
run_debug:
	@qemu-system-aarch64 -smp 4 -S -M virt,secure=on,virtualization=on \
	 -cpu cortex-a72 -nographic -bios \
	 $(PROJECT).elf -gdb tcp::1234 \
	 -device loader,addr=0x40100000,cpu-num=0 \
     -device loader,addr=0x40100000,cpu-num=1 \
	 -device loader,addr=0x40100000,cpu-num=2 \
	 -device loader,addr=0x40100000,cpu-num=3 \
	 -device loader,file=$(PROJECT).elf

debug:
	@gdb-multiarch shell.elf -ex "target remote localhost:1234"

run: $(PROJECT).elf
	@qemu-system-aarch64 -smp 4 -M virt,secure=on,virtualization=on \
	 -cpu cortex-a72 -nographic \
	 -bios $(PROJECT).elf \
	 -device loader,addr=0x40100000,cpu-num=0 \
     -device loader,addr=0x40100000,cpu-num=1 \
	 -device loader,addr=0x40100000,cpu-num=2 \
	 -device loader,addr=0x40100000,cpu-num=3 \
	 -device loader,file=$(PROJECT).elf