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

#
# Acquire lock using load-/store-exclusive instruction pair.
#
# void acquire(uint_64 *lock);
#

.globl acquire
acquire:
    mov     w2, #1
    sevl                           // SEVL means that the first time we hit the WFE it will "wake" immediately
l1: wfe                            // Enter the sleep mode
l2: ldaxr   w1, [x0]               // memory access is atomic and physical address is marked as exclusive.
    cbnz    w1, l1                 // if lock->locked == 1, go to wfe
    stxr    w1, w2, [x0]           // atomic store of w2 value(1) in exclusive marked address and on success w1 = 0 else w1 = 1
    cbnz    w1, l2                 // if store was not succesful loop again
    ret

#
# Release lock previously acquired by spin_lock.
#
# Use store-release to unconditionally clear the spinlock variable.
# Store operation generates an event to all cores waiting in WFE.
#
# void release(uint_64 *lock);
#

.globl release
release:
    stlr    wzr, [x0]
    ret

#
# Returns cpuid of the core
#
# uint_64 mycpu(void);
#

.globl mycpu
mycpu:
    mrs     x0, mpidr_el1          // get core Id
    and     x0, x0, #0x3           // Make cpu id bitmask
    ret