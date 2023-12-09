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

#define ISR_HANDLER(x) ((uint32_t)&x)

extern void reset_handler(void);

void default_handler(void) {
  while (1)
    ;
}

uint32_t vector_table[240] __attribute__((section(".init_vector"))) = {
    (uint32_t)STACK_START, ISR_HANDLER(reset_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), 0, 0, 0, 0, ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler), 0, 0,
    // 132 IRQs
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler),
    ISR_HANDLER(default_handler), ISR_HANDLER(default_handler)};
