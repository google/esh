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

# User Linker Scripts

There is a provision for the users to be able to provide their own memory layout
 (linker scripts) if they choose to. One needs to simple set the `USER_LAYOUT_FILE`
in the project level makefile. This variable should point to the user defined
memory layout (linker script) file.

Consider for example the following preoject layout

```shell
examples/physical/hifive
├── Makefile
└── platform
    ├── layout.ld
    ├── platform.c
    ├── riscv32.S
    └── uart
        ├── uart.c
        └── uart.h
```

`examples/physical/hifive/platform/layout.ld` is the user defined custom memory
layout file. The build system is instructed to pick this file over the inferred
layout file by setting the `USER_LAYOUT_FILE` variable in the
`examples/physical/hifive/Makefile` as below:

```make
...
USER_LAYOUT_FILE = platform/layout.ld
...
```

# Mandatory symbols and sections

In order for the shell to work correctly, the user memory layout file (linker
script) must expose some symbols and define some sections. These symbols are
**essential for the shell to work correctly**. Specifically because the shell
depend on these to configure itself and find where the command table is.

## Sections: To look up commands

Following sections are mandatory so the shell can find the commands:
1. `*(.cmd_list*)` - The commands added using the ADD_CMD() macro go in this
section. Shell looks for for a given string in this region to search the
associated command to be execute on a string match.
1. `*(.auto_list*)` - The commands added using the AUTO_CMD() macro go in this
section. Shell will automatically execute the commands in this section
immediately after boot, before the prompt is presented for the user
interactions.
1. `*(.cmd_end*)` - This section consist of `12 Bytes` worth of `0`s and marks
the end of the command table. The shell infers the end of command table based
on this section. This is important for the mechanics of the command search
to work reliable.

Placement of the above sections must be made at the `8 Byte` aligned memory
address and these be placed together as follows:
```ld
. = ALIGN(8);
*(.cmd_list*)
*(.auto_list*)
*(.cmd_end*)
```
The order is important! **These must go in the exact same order!**

Since the command table is supposed to be immutable during the runtime
these sections should be placed in the readonly sections of the memory. For
example the `.rodata` section. Example placement is as below:
```ld
.rodata : {
...
    . = ALIGN(8);
    *(.cmd_list*)
    *(.auto_list*)
    *(.cmd_end*)
...
```

## Symbols: For shell init

Certain symbols orient the shell and let's it do
- The initialization correctly.
- Be aware of where the command database is.
- Initialize the stack correctly.

Complete list of symbols is as follows:
1. `_text`: points to start of the `.text` section.
1. `_etext`: points to end of the `.text` section.
1. `_data`: points to the start of the `.data` section.
    1. preferrably this should start at an `8 Byte` boundary.
1. `_edata`: points to the end of the data sections.
1. `_bss`: points to starts of the `.bss` sections.
    1. preferrably this should start at an `8 Byte` boundary.
1. `_ebss`: points to starts of the `.bss` sections.
1. `__CMD_TABLE_START__`: points to the start of `.cmd_list*` section.
    1. Shell uses this symbol to infer the address at which the command database/table starts.
1. `__AUTO_TABLE_START__`: points to the starts of `.auto_list*` section.
    1. After the boot, shell starts to execute all the commands placed in the
`.auto_list*` section. It uses this symbol to infer the address of the start of
the list of commands to be executed automatically.
1. `_STACK_TOP_`: points to the start of the stack.

---

A typical linker script section placement would look as follows:
```ld
...
SECTIONS
{
    .text : {
        _text = .;

        *(.init*)
        *(.text*)

        _etext = .;
    } > ROM AT> ROM

    .data : {
        _data = .;

        . = ALIGN(8);
        *(.data*)

        _edata = .;
    } > RAM AT> ROM

    .rodata : {
        /* start: Command table for the shell */
        . = ALIGN(8);
        __CMD_TABLE_START__ = .;
        *(.cmd_list*)
        __AUTO_TABLE_START__ = .;
        *(.auto_list*)
        *(.cmd_end*)
        /* end: Command table for the shell */

        *(.rodata*)
    } > ROM AT> ROM

    .bss : {
        _bss = .;
        . = ALIGN(8);
        *(.bss*)
        _ebss = .;
    } > RAM AT> RAM

    PROVIDE(_STACK_TOP_ = ORIGIN(RAM) + LENGTH(RAM) - 4);
}
```

An exmaple of a complete user defined linker file can look as follows (this can
also be used as a template to start with and sections may be appended as
 needed):

```ld
MEMORY
{
    ROM (rx) : ORIGIN = 0x0, LENGTH = 0x10000
    RAM (rwx) : ORIGIN = 0x2000, LENGTH = 0x10000
}

SECTIONS
{
    .text : {
        _text = .;

        *(.init*)
        *(.text*)

        _etext = .;
    } > ROM AT> ROM

    .data : {
        _data = .;

        . = ALIGN(8);
        *(.data*)

        _edata = .;
    } > RAM AT> ROM

    .rodata : {
        /* start: Command table for the shell */
        . = ALIGN(8);
        __CMD_TABLE_START__ = .;
        *(.cmd_list*)
        __AUTO_TABLE_START__ = .;
        *(.auto_list*)
        *(.cmd_end*)
        /* end: Command table for the shell */

        *(.rodata*)
    } > ROM AT> ROM

    .bss : {
        _bss = .;
        . = ALIGN(8);
        *(.bss*)
        _ebss = .;
    } > RAM AT> RAM

    PROVIDE(_STACK_TOP_ = ORIGIN(RAM) + LENGTH(RAM) - 4);
}
```
`ROM` and `RAM` origin and length can vary.
