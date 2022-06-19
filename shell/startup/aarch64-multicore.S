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

#define CPU_STACK_SIZE 1024
#define PLATFORM_CORE_COUNT 4
#define WATCH_VALUE_SIZE 8
# Execution starts from here
.section .init
.globl _start_
_start_:
    //*********************************************//
    // Setup Stack pointer for each core           //
    //*********************************************//
    mrs x0, mpidr_el1                    // Read Core id on Armv8
    and x0, x0, #0x3                     // Make cpu id bitmask
    ldr x13, =_STACK_TOP_                // load address of stack top into x13
    mov x2, #CPU_STACK_SIZE              // each core gets stack size of 1024
    mul x1, x0, x2                       // allocating stack according to core id
    sub x13, x13, x1                     // x13 will be sp if we switch to aarch32
    mov sp, x13
    adr x5, spin_cpu                     // Load address of spin cpu
    mov x1, #0                           // zero x1 register
    mov x2, #WATCH_VALUE_SIZE
    mul x2, x2, x0                       // offset = (core_num)*WATCH_VALUE_SIZE
    str x1, [x5, x2]                     // Make sure caller address is zeroed, caller address = watch base address + offset
    cbz x0, 1f                           // core 0 branches and secondary cores enter the spin loop
 //*************************************************//
 //                   spinloop                      //
 //**************************************************//

StartSecondarySpin:
    mrs x6, mpidr_el1                    // get core Id
    and x6, x6, #0x3                     // Make cpu id bitmask
    adr x5, spin_cpu                     // Load address of spin cpu
    mov x2, #WATCH_VALUE_SIZE
    mul x2, x2, x6                       // offset = (core_num)*WATCH_VALUE_SIZE
    adr x7, core_available                // Load address of core_available
    mov x8, #1
    str x8, [x7, x2]                     // Store 1 at (offset + base address), to make sure core is ready & avilable
secondary_spin:
    wfe                                  // secondary cores goes to sleep
    ldr x4, [x5, x2]                     // Add offset to the base address and load what's there into x4
    cbz x4, secondary_spin               // Loop if zero, otherwise continue
    mov x0, #0
    str x0, [x5, x2]                     // Zero caller address
    str x0, [x7, x2]                     // Store 0 at (offset + base address), to make sure core is not avilable
    mov x1, #0
    mov x2, #0                           // Zero registers x0-x3, just in case
    mov x3, #0
    mov x0, x6                           // now x0 holds cpu id before it jumps to function
    blr x4                               // jump to the address in x4 , and start excecuting the instruction there
    b StartSecondarySpin                 // Loop back to spin lock

1:                                       // Core 0 branches to prompt
    bl prompt                            // show the prompt
    b .

.ltorg

.org 0xd8
.globl spin_cpu                                 // watch base address
spin_cpu:
    .skip WATCH_VALUE_SIZE*PLATFORM_CORE_COUNT  // uninitialized array of size PLATFORM_CORE_COUNT , each core watch a value of size WATCH_VALUE_SIZE bytes

.globl core_available                           // core_available stores availablity of each platform core for execution.
core_available:
    .skip WATCH_VALUE_SIZE*PLATFORM_CORE_COUNT  // uninitialized array of length PLATFORM_CORE_COUNT , each element of size WATCH_VALUE_SIZE bytes
