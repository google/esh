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

**[<< Back to TOC](index.md)**

---
# Platform Initialization
Be sure to read [01-setting-up-new-project.md](01-setting-up-new-project.md) before continuing as this document will make references to it and assuming that the reading is aware of the background as to why platform initialization is required.

Platform initialization is a way for the user to do one time configuration of the hardware and is done by implementing `platform_init()` functions. The initialization could be for example - clock configurations, enabling or disabling certain blocks etc. **If not anything, the user needs to provide a way for shell to put and get a character from some stream (e.g uart). This is Mandatory! And expected!**

`platform_init()` can be implemented at any level but for the reasons of making the source code organization obvious, it's preferred to have a `platform.c` file which implements it. As discussed in [Define platform_init()](01-setting-up-new-project.md#define-platforminit) section we have the following directory structure:
```
new_project
|-- Makefile
└── platform.c
```
With `platform.c` having the following:
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
- Our interest is in populating the contents of `pltform_init()`. As hinted earlier the shell needs a way to put and get a character from somewhere so user interaction is possible.
- Typically this would be a `uart` and in certain cases can be `memory`, `mailbox` or some other interface. We assume it is the `uart`.

## The Uart interfaces
The user would need to provide a uart driver that provides the following interfaces:
- `void uart_init(void)` uart initialization.
- `void uart_putc(char c)` a way to send a character out.
- `int uart_getc(void)` a way to read a character in.

More details on the uart driver are discussed in the [03-uart-driver.md](03-uart-driver.md) documentation.

Assuming these implementations to be available through `uart.h` (that the user has separately implemented), the `platform.c` would then look like so -

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
#include "uart.h"

void platform_init(void) {
  /**
   * User can do other configurations here.
   * E.g clocks, enable/disable blocks
   */

  /* Initialize the UART */
  uart_init();

  /* Register with shell, a function to get a character from the uart */
  set_read_char(uart_getc);

  /* Register with shell, a function to send a character to the uart */
  set_write_char(uart_putc);
}
```
- Issuing `make` in `new_project/` should result in the following:
```
$ make
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
/home/itankar/early-bringup-tool/new_project/platform.c:18:10: fatal error: uart.h: No such file or directory
   18 | #include "uart.h"
      |          ^~~~~~~~
compilation terminated.
make: *** [../shell/Makefile:148: /home/itankar/early-bringup-tool/new_project/platform.o] Error 1
```
- we would need to fix this. Create a directory `new_project/uart/` and add two files `uart.c` and `uart.h` in it. (if the user already has a uart driver, then the source should be simply copied to `new_project/` and the expected interfaces as expected above should be provided in `uart.h`).
- the directory structure should look as follows:
```
new_project
|-- Makefile
|-- platform.c
└──  uart
    |-- uart.c
    └── uart.h
```
- put the following in `uart.h`
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

#ifndef __UART_H__
#define __UART_H__

void uart_init(void);
void uart_putc(char c);
int uart_getc(void);

#endif
```
- put the following in `uart.c`
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

#include "uart.h"

void uart_init(void) {

}

void uart_putc(char c) {

}

int uart_getc(void) {
  return -1;
}
```
- Issue `make` in `new_project/` again. Prints similar to following should be seen:
```
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

compiling /home/itankar/early-bringup-tool/new_project/uart/uart.c
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
   5576    1112      88    6776    1a78 shell.elf
generating shell.bin
Done!
```
# Next:
- While the compilation is successful. Becuase the uart driver in this case was incomplete (empty functions), the shell would boot but now work as expected.
- [03-uart-driver.md](03-uart-driver.md) discusses more on the uart driver implementation which should be the last step in making the shell completely functional.

---
**[<< Back to TOC](index.md)**

---
