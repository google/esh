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
        *(.cmd_list*)
        __AUTO_TABLE_START__ = .;
        *(.auto_list*)
        *(.cmd_end*)
        *(.rodata*)
    } > RAM

    /**
     * Let the stack be at the end of the RAM.
     * Not all CPUs may decrement the stack before use,
     * so point to one word less than the end.
     */
    _STACK_TOP_ = __RAM_BASE__ + __RAM_SIZE__ - 4;
}
