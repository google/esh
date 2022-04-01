<!--

Copyright 2022 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

-->

**[<< Back to TOC](00-getting-started.md)**

---
# Setting up new Project
At the root level create a direcoty `new_project`. The repo should have the following structure. For the purposes of this guide, we set up a from the scratch project in `new_project/` directory.
```
.
├── docs
├── examples
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
# Copyright <year> <author>
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

# Set this as 1 to use a lite version of the shell \
# This disables support for the following: \
#   * printf formatting \
#   * long integers in printf \
#   * memory manipulation utilities \
#   * commmand history in prompt \
# Skip the next 4 defines in this file if using SHELL_LITE
# SHELL_LITE = 1

# Uncomment to use printf() without the formatting support
# DEFINES += -DSHELL_PRINTF_LITE

# Uncomment to use printf() without the long interger
# formatting support. Valid only if SHELL_PRINTF_LITE is not defined
# DEFINES += -DSHELL_NO_PRINTF_LL

# Uncomment to skip compiling the memory manipulation utilities.
# DEFINES += -DSHELL_NO_UTILS

# Uncommnet to disable the command history on the prompt
# DEFINES += -DSHELL_NO_HISTORY

# Set this variable to point to the shell directory
SHELL_ROOT=../shell
-include $(SHELL_ROOT)/Makefile

# ------------------------ User targets below this line ------------------------

# example: <dependencies>
#     @recipes
```

## Note
- The comments in the `Makefile` are self explanatory.
- One can also look at the Makefiles as part of the examples implemented in the `examples/` directory.
- The purpose of the project level `Makefile` is to the connect with the shell's `Makefile` (`shell/Makefile`) and use all the magic available there.
  - A study of the `shell/Makefile` should give more insight into how the build process works.
  - The Project level variables steer the build magic to achieve the compilation.

## Try to compile
- Issuing the `make` command should result in the following prints.
```
➜   make
Detected Configuration
*
|- PROJECT             : shell
|- BUILD USER          : itankar
|- BUILD HOST          : sandbox
|- SHELL VERSION       : a2dc94e
|- USER REPO VERSION   : a2dc94e
|- SHELL ROOT          : /home/itankar/early-bringup-tool/shell
|- PROJECT ROOT        : /home/itankar/early-bringup-tool/new_project
|- OPTIMIZATION        : 0
*
|- TOOLCHAIN           : riscv64-linux-gnu-
|- STARTUP             : /home/itankar/early-bringup-tool/shell/startup/riscv64.S
|- LAYOUT FILE         : /home/itankar/early-bringup-tool/shell/scatter/layout.ld
|- RAM BASE            : 0x80000000
|- RAM SIZE            : 0x4000000
`- UART BASE           : 0x10000000

>> If incorrect, please set these Variables in Makefile <<

compiling /home/itankar/early-bringup-tool/shell/lib/utils/utils.c
compiling /home/itankar/early-bringup-tool/shell/lib/string/string.c
compiling /home/itankar/early-bringup-tool/shell/lib/a2i/a2i.c
compiling /home/itankar/early-bringup-tool/shell/lib/printf/printf.c
compiling /home/itankar/early-bringup-tool/shell/shell.c
assembling /home/itankar/early-bringup-tool/shell/startup/riscv64.S
generating /home/itankar/early-bringup-tool/shell/scatter/layout.ld
generating shell.elf
riscv64-linux-gnu-ld: /home/itankar/early-bringup-tool/shell/shell.o: in function `exec_auto_cmds':
/home/itankar/early-bringup-tool/shell/shell.c:280: undefined reference to `platform_init'
make: *** [../shell/Makefile:156: shell.elf] Error 1
```
- The linking phase fails! the linker (`ld`) has reported a missing implementation of `platform_init()`
```
riscv64-linux-gnu-ld: /home/itankar/early-bringup-tool/shell/shell.o: in function `exec_auto_cmds':
/home/itankar/early-bringup-tool/shell/shell.c:280: undefined reference to `platform_init'
make: *** [../shell/Makefile:156: shell.elf] Error 1
```

# Define platform_init()
- Do get to zero error compilation we would need to define a function `platform_init()`.
- To do so create another file `platform.c` and add the following to it:
```
/**
 * Copyright <year> <author>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "shell.h"

void platform_init(void) {

}
```
- At this point the directory structure should be as follows:
```
new_project
|-- Makefile
└── platform.c
```
- Issue `make` again and you should see similar prints as below:
```
➜   make
Detected Configuration
*
|- PROJECT             : shell
|- BUILD USER          : itankar
|- BUILD HOST          : sandbox
|- SHELL VERSION       : a2dc94e
|- USER REPO VERSION   : a2dc94e
|- SHELL ROOT          : /home/itankar/early-bringup-tool/shell
|- PROJECT ROOT        : /home/itankar/early-bringup-tool/new_project
|- OPTIMIZATION        : 0
*
|- TOOLCHAIN           : riscv64-linux-gnu-
|- STARTUP             : /home/itankar/early-bringup-tool/shell/startup/riscv64.S
|- LAYOUT FILE         : /home/itankar/early-bringup-tool/shell/scatter/layout.ld
|- RAM BASE            : 0x80000000
|- RAM SIZE            : 0x4000000
`- UART BASE           : 0x10000000

>> If incorrect, please set these Variables in Makefile <<

compiling /home/itankar/early-bringup-tool/new_project/platform.c
compiling /home/itankar/early-bringup-tool/shell/lib/utils/utils.c
compiling /home/itankar/early-bringup-tool/shell/lib/string/string.c
compiling /home/itankar/early-bringup-tool/shell/lib/a2i/a2i.c
compiling /home/itankar/early-bringup-tool/shell/lib/printf/printf.c
compiling /home/itankar/early-bringup-tool/shell/shell.c
assembling /home/itankar/early-bringup-tool/shell/startup/riscv64.S
generating /home/itankar/early-bringup-tool/shell/scatter/layout.ld
generating shell.elf
section size:
   text    data     bss     dec     hex filename
   5494    1096      88    6678    1a16 shell.elf
generating shell.bin
Done!
```
# Next
- **A zero error compilation with project level `Makefile` (as below) doesn't necessarily mean that the binary will boot and the shell would be available on the uart**.
  - for the shell to work correctly it also needs a uart driver. Typically the user has to implement this.
  - the `platform_init()` funciton initializes the `uart` and provides the shell with a means of putting and fetching a character from the UART.
- See [02-platform_init-and-uart-driver.md](02-platform_init-and-uart-driver.md) for details on how to enable shell to be fully functional.

---
**[<< Back to TOC](00-getting-started.md)**

---
