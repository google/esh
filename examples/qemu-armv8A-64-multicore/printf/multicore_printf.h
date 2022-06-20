/*
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
*/

#include "printf.h"
#include "spinlock.h"

// a spinlock with a switch to turn locking on/off
typedef struct lock_t {
  struct spinlock lock;
  int locking;
} lock_t;

extern struct lock_t pr;

// provides a spinlock wrapper around printf() to avoid interleaving concurrent
// printf's.
#define multicore_printf(fmt, ...) \
  do {                             \
    if (pr.locking) {              \
      spin_lock(&pr.lock);         \
    }                              \
    printf(fmt, ##__VA_ARGS__);    \
    if (pr.locking) {              \
      spin_unlock(&pr.lock);       \
    }                              \
  } while (0)

void printfinit(void);