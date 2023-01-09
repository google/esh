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
    RAM : ORIGIN = __RAM_BASE__, LENGTH = __RAM_SIZE__
}

/* 
 * Symbols used for initialising data. While this is not required when everything is
 * in RAM, the symbols are defined to keep the code consistent.
 */
_etext = LOADADDR(.data);
_data = ADDR(.data);
_edata = ADDR(.data) + SIZEOF(.data);

/* Symbols used for zero-ing out BSS */
_bss = ADDR(.bss);
_ebss = ADDR(.bss) + SIZEOF(.bss);

SECTIONS
{
    .text : {
        *(.init*)
        *(.text*)
    } > RAM

    .data : {
        . = ALIGN(8);
        *(.data*)
    } > RAM

    .bss : {
        . = ALIGN(8);
        *(.bss*)
    } > RAM

    .rodata : {
        . = ALIGN(8);
        __CMD_TABLE_START__ = .;
        KEEP(*(.cmd_list*))
        __AUTO_TABLE_START__ = .;
        KEEP(*(.auto_list*))
        KEEP(*(.cmd_end*))
        *(.rodata*)
    } > RAM

    /**
     * Let the stack be at the end of the RAM.
     * Not all CPUs may decrement the stack before use,
     * so point to one word less than the end.
     */
    PROVIDE(_STACK_TOP_ = ORIGIN(RAM) + LENGTH(RAM) - 4);
}
