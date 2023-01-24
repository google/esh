#
# Copyright 2021 Google LLC
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

# Execution starts from here
.section .init
.globl _start_
_start_:
    // Only boot one cpu, others spin!
    mrs x0, mpidr_el1
    and x0, x0, #0x3
    cbz x0, 1f
    b .
1:

    mrs x1, cptr_el3
    bic x1, x1, #(1 << 10) // Ensure that CPTR_EL3.TFP is clear
    msr cptr_el3, x1
    mrs x1, cptr_el2
    bic x1, x1, #(1 << 10)
    msr cptr_el2, x1

    mov x1, #(0x3 << 20)
    msr cpacr_el1, x1

    isb sy

    ldr x0, =_STACK_TOP_    // Store the stackpointer
    mov sp, x0              // Move the value to SP
    bl prompt               // show the prompt
    b .
