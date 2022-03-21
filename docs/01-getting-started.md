# Getting Started

# Directory structure
At the root level create a direcoty `new_project`. The repo should have the following structure. For the purposes of this guide, we set up a from the scratch project in `new_project/` directory.
```
.
├── docs
├── example
├── LICENSE
├── new_project
├── README.md
└── shell
```

# Setting up the biuld - Makefile
In the project directory (`new_project/`), create a `Makefile`. The directory tree should look as follows:
```
new_project
└── Makefile
```
Add following to the `new_project/Makefile`

```
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

# Set the toolchain prefix, something similar to below.
# this example is for RISC-V, For ARM it would be -
# arm-none-eabi- or aarch64-linux-gnu-
TOOLCHAIN_PREFIX = riscv64-linux-gnu-

# This option affects the optimization that the compiler will use
OPTIMIZATION     = 0

# Set the startup file that should be used for compilation.
# This should be a .S file that has the initialization code.
# user can either provide name of a file already available in
# shell/startup/ or give name of a user defined file.
STARTUP    		 = riscv64

# Provide the base of the boot memory
RAM_BASE_PHYSICAL  = 0x80000000

# Provide the size of the memory
RAM_SIZE           = 0x4000000

# Provide the base of the uart on which the shell
# should be spwaned on.
UART_BASE_PHYSICAL = 0x10000000

# If you have your own layout file that should be used by the linker, set this
# variable to point to the layout file.
# USER_LAYOUT_FILE = layout.ld

# DEFINES   += <flags to be passed to compiler>
# ASM_FLAGS += <flags to be passed to the assembler>
# LD_FLAGS  += <flags to be passed to the assembler>

# Uncomment to use printf() without the formatting support
# DEFINES += -DSHELL_PRINTF_LITE

# Uncomment to use printf() without the long interger
# formatting support. Valid only if SHELL_PRINTF_LITE is not defined
# DEFINES += -DSHELL_NO_PRINTF_LL

# Uncomment to skip compiling the memory manegement utilities.
# DEFINES += -DSHELL_NO_UTILS

# Uncommnet to disable the command history on the prompt
# DEFINES += -DSHELL_NO_HISTORY

# Set this variable to point to the shell directory
SHELL_ROOT=../shell
-include $(SHELL_ROOT)/Makefile

# ------------------------ User targets below this line ------------------------

# example: <dependencies>
#     @recepies
```

## Note
- The comments in the `Makefile` are self explanatory.
- One can also look at the Makefiles as part of the examples implemented in the `examples/` directory.
- The purpose of the project level `Makefile` is to the connect with the shell's `Makefile` (`shell/Makefile`) and use all the magic available there.
  - A study of the `shell/Makefile` should give more insight into how the build process works.
  - The Project level variables steer the build magic to achieve the compilation.

## Warning
- **A zero error compilation with project level `Makefile` (as below) doesn't necessarily mean that the binary will boot and the shell would be available on the uart**.
  - for the shell to work correctly it also needs a uart driver. Typically the user has to implement this.
```
➜ make
Detected Configuration
*
|- PROJECT             : shell
|- BUILD USER          : itankar
|- BUILD HOST          : itankar.blr.corp.google.com
|- SHELL VERSION       : 122ff0c
|- USER REPO VERSION   : 122ff0c
|- SHELL ROOT          : /usr/local/google/home/itankar/Downloads/early-bringup/shell
|- PROJECT ROOT        : /usr/local/google/home/itankar/Downloads/early-bringup/demo
|- OPTIMIZATION        : 0
*
|- TOOLCHAIN           : riscv64-linux-gnu-
|- STARTUP             : /usr/local/google/home/itankar/Downloads/early-bringup/shell/startup/riscv64.S
|- LAYOUT FILE         : /usr/local/google/home/itankar/Downloads/early-bringup/shell/scatter/layout.ld
|- RAM BASE            : 0x80000000
|- RAM SIZE            : 0x4000000
`- UART BASE           : 0x10000000

>> If incorrect, please set these Variables in Makefile <<

compiling /usr/local/google/home/itankar/Downloads/early-bringup/shell/lib/string.c
compiling /usr/local/google/home/itankar/Downloads/early-bringup/shell/lib/utils.c
compiling /usr/local/google/home/itankar/Downloads/early-bringup/shell/lib/printf.c
compiling /usr/local/google/home/itankar/Downloads/early-bringup/shell/lib/a2i.c
compiling /usr/local/google/home/itankar/Downloads/early-bringup/shell/platform_init.c
compiling /usr/local/google/home/itankar/Downloads/early-bringup/shell/shell.c
assembling /usr/local/google/home/itankar/Downloads/early-bringup/shell/startup/riscv64.S
generating /usr/local/google/home/itankar/Downloads/early-bringup/shell/scatter/layout.ld
generating shell.elf
section size:
   text    data     bss     dec     hex filename
   5608    1128      88    6824    1aa8 shell.elf
generating shell.bin
Done!
```

