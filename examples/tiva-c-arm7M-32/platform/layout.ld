/**
 * Copyright 2022 Google LLC
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

MEMORY
{
    ROM (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

SECTIONS
{
    .text : {
        _text = .;

        *(.init*)
        *(.text*)

        /* start: Command table for the shell */
        . = ALIGN(8);
        __CMD_TABLE_START__ = .;
        *(.cmd_list*)
        __AUTO_TABLE_START__ = .;
        *(.auto_list*)
        *(.cmd_end*)
        /* end: Command table for the shell */

        *(.rodata*)
        _etext = .;
    } > ROM

    .data : AT(ADDR(.text) + SIZEOF(.text)){
        _data = .;
        . = ALIGN(8);
        *(.data*)
        _edata = .;
    } > RAM

    .bss : {
        _bss = .;
        . = ALIGN(8);
        *(.bss*)
        _ebss = .;
    } > RAM

    PROVIDE(_STACK_TOP_ = ORIGIN(RAM) + LENGTH(RAM) - 4);
}
