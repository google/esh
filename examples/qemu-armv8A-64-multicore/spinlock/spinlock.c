/**
 * Copyright 2021 Google LLC
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

#include "printf.h"
#include "spinlock.h"

// signals if any core tried re-acquiring or re-releasing the lock
volatile int re_entrancy = 0;

// initialise the spinlock, with given name
void initlock(struct spinlock *lock, char *name) {
  lock->name = name;
  lock->locked = 0;
  lock->cpu = -1;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
void spin_lock(struct spinlock *lock) {
  // spin_lock is not re-entrant
  // if cpu itself is holding lock, signal to freeze each platform core
  if (holding(lock)) {
    re_entrancy = 1;
  }
  // if re_entrancy == 1 , freeze the core
  if (re_entrancy) {
    freeze_cpu();
  }
  // acquire lock->locked , loop (spins) untill the lock is acquired.
  acquire(&lock->locked);
  // Record info about lock acquisition for holding() and debugging.
  lock->cpu = mycpu();
}

// Release the lock.
void spin_unlock(struct spinlock *lock) {
  // spin_lock is not re-entrant
  // if cpu is not holding the lock at first place, signal to freeze each
  // platform core
  if (!holding(lock)) {
    re_entrancy = 1;
  }
  // if freeze == 1, freeze the core
  if (re_entrancy) {
    freeze_cpu();
  }

  lock->cpu = -1;
  // Release the lock, equivalent to lock->locked = 0.
  release(&lock->locked);
}

// Check whether this cpu is holding the lock.
int holding(struct spinlock *lock) {
  int r;
  r = (lock->locked && lock->cpu == mycpu());
  return r;
}

// send freeze message to console and enter infinite loop (freeze)
void freeze_cpu(void) {
  printf("Core %ld is freezed.\n", mycpu());
  // freeze
  for (;;) {
    asm volatile("wfe");
  }
}