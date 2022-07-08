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

#include "regs.h"
#include "shell.h"

/*
 * Memory manipulation utilities can be skipped to reduce
 * final binary size. Skip if SHELL_NO_UTILS is defined.
 */
#ifndef SHELL_NO_UTILS
int r32(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <address> (in hex)\n", argv[0]);
    return -1;
  }

  uint32_t addr = atoh(argv[1]);
  uint32_t data;

  data = reg32(addr);

  printf("0x%x: 0x%x\n", addr, data);
  return 0;
}

int w32(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <address> (in hex) <value> (in hex)\n", argv[0]);
    return -1;
  }
  uint32_t addr = atoh(argv[1]);
  uint32_t data = atoh(argv[2]);
  reg32(addr) = data;

  return 0;
}

int read_mem(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <address> (in hex) <num_words> (in decimal)\n", argv[0]);
    return -1;
  }

  uint32_t addr = atoh(argv[1]);
  uint32_t length = atoi(argv[2]);
  uint32_t data;

  for (uint32_t i = 0; i < length; i++) {
    data = reg32(addr);
    printf("0x%x: 0x%x\n", addr, data);

    addr += 4;
  }

  return 0;
}

ADD_CMD(r32, "reads a 32 bit memory location", r32);
ADD_CMD(w32, "writes a 32 bit value to a memory location", w32);
ADD_CMD(read, "Reads number of bytes from memory", read_mem);
#endif  // SHELL_NO_UTILS
