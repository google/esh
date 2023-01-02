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

  for (uint32_t i = 0, data = 0; i < length; i++) {
    data = reg32(addr);
    printf("0x%x: 0x%x\n", addr, data);

    addr += 4;
  }

  return 0;
}

#ifndef SHELL_NO_BIT_UTILS
int w32_bit(int argc, char *argv[])
{
  if (argc < 4) {
    goto usage;
  }

  uint32_t addr = atoh(argv[1]);
  uint32_t bit_loc = atoi(argv[2]);
  uint32_t bit_val = atoi(argv[3]);

  if (bit_loc > 31) {
    printf("Error: Invalid bit location argument\n");
    goto usage;
  }

  if (bit_val != 0 && bit_val != 1) {
    printf("Error: Invalid bit value argument\n");
    goto usage;
  }

  uint32_t data = reg32(addr);
  data = data | (bit_val << bit_loc);
  data = data & ~(!bit_val << bit_loc);
  reg32(addr) = data;

  return 0;
usage:
  printf("Usage: %s <address_in_hex(32-bits)> <bit_location_in_decimal(0 to 31)> <bit_value(0 or 1)>\n", argv[0]);
  return -1;
}

int r32_bit(int argc, char *argv[])
{
  if (argc < 3) {
    goto usage;
  }

  uint32_t addr = atoh(argv[1]);
  uint32_t bit_loc = atoi(argv[2]);

  if (bit_loc > 31) {
    printf("Error: Invalid bit location argument\n");
    goto usage;
  }

  uint32_t data = reg32(addr);
  uint32_t bit_val = (data >> bit_loc) & 0x1;
  printf("Bit %u @ 0x%08x: %u\n", bit_loc, addr, bit_val);
  return 0;

usage:
  printf("Usage: %s <address_in_hex(32-bits)> <bit_location_in_decimal(0 to 31)\n", argv[0]);
  return -1;
}

ADD_CMD(wb, "write a bit to memory location", w32_bit);
ADD_CMD(rb, "read a bit to memory location", r32_bit);
#endif //SHELL_NO_BIT_UTILS

ADD_CMD(r32, "reads a 32 bit memory location", r32);
ADD_CMD(w32, "writes a 32 bit value to a memory location", w32);
ADD_CMD(read, "Reads number of bytes from memory", read_mem);
#endif  // SHELL_NO_UTILS
