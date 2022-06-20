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

#include <stdint.h>

extern void acquire(uint64_t *);
extern void release(uint64_t *);
extern uint64_t mycpu(void);

// Mutual exclusion lock.
struct spinlock {
  uint64_t locked;  // Is the lock held?

  // For debugging:
  char *name;    // Name of lock.
  uint64_t cpu;  // The cpu holding the lock.
};

void initlock(struct spinlock *, char *);
void spin_lock(struct spinlock *);
void spin_unlock(struct spinlock *);
int holding(struct spinlock *);
void freeze_cpu(void);