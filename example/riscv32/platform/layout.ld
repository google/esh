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
    ROM : ORIGIN = 0x20010000, LENGTH = 0x1000
    RAM : ORIGIN = 0x80000000, LENGTH = 0x4000
}

SECTIONS
{
    .text : {
        *(.init*)
        *(.text*)
    } > ROM

    .data : {
        . = ALIGN(8);
        *(.data*)
    } > ROM

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
    } > ROM

    /**
     * Let the stack be at the end of the RAM.
     * Not all CPUs may decrement the stack before use,
     * so point to one word less than the end.
     */
    _STACK_TOP_ = 0x80000000 + 0x4000 - 4;
}
