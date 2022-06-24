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

#
# Acquire lock using load-/store-exclusive instruction pair.
# Get a ticket number and check and loop untill its your turn to get served/processed.
#
# void acquire(uint_64 *next_ticket, uint_64 *curr_ticket);
#

.globl acquire
.type acquire, %function

acquire:
GetTicket:
    ldaxr    x3, [x0]               // load the next ticket avilable
    add      x4, x3, #1             // x3 holds the ticket allocated to this core, we bump the next ticket
    stxr     w5, x4, [x0]           // if updating next ticket succeds, this core gets the allocated ticket in x3
    cbnz     w5, GetTicket          // check if store succeded and retry if not
    sevl                            // SEVL means that the first time we hit the WFE it will "wake" immediately
CheckTicket:
    wfe                             // enter sleep mode
    ldaxr    x2, [x1]               // load the current ticket number, load must be exclusive so event is triggered when exclusive state is cleared
    sub      x5, x3, x2             // x5 = my_ticket - curr_ticket
    cbnz     x5, CheckTicket        // if x5 == 0, this core enters the critical section or else loop back to check ticket.
    ret

#
# Release lock previously acquired by spin_lock.
#
# Use store-release to unconditionally increment the current ticket number variable.
# Store operation generates an event to all cores waiting in WFE.
#
# void release(uint_64 *curr_ticket);
#

.globl release
.type release, %function

release:
    ldr      x3, [x0]               // load current ticket number
    add      x3, x3, #1             // increment current ticket number
    stlr     x3, [x0]               // store it back and signal sleeping cores to check ticket.
    ret

#
# Returns cpuid of the core
#
# uint_64 mycpu(void);
#

.globl mycpu
.type mycpu, %function

mycpu:
    mrs      x0, mpidr_el1          // get core Id
    and      x0, x0, #0x3           // Make cpu id bitmask
    ret
