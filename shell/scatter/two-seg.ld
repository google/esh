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
    /**
     * Use the SRAM base and Size provided by the user in the Makefile
     * __RAM_BASE__ and __RAM_SIZE__ will be dynamically replaced during the build.
     */
    ROM (rx) : ORIGIN = __ROM_BASE__, LENGTH = __ROM_SIZE__
    RAM (rwx) : ORIGIN = __RAM_BASE__, LENGTH = __RAM_SIZE__
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

    /**
     * Let the stack be at the end of the RAM.
     * Not all CPUs may decrement the stack before use,
     * so point to one word less than the end.
     */
    PROVIDE(_STACK_TOP_ = ORIGIN(RAM) + LENGTH(RAM) - 4);
}
