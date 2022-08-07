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

#include "memlog.h"
#include "string.h"

static rgn_info_t inp_buf_rgn, out_buf_rgn;
static mode_t selected_mode = NONE;

void memlog_init(uint8_t *addr, uint32_t size, mode_t mode) {
  selected_mode = mode;

  if (selected_mode == NONE) {
    return;
  }

  out_buf_rgn.addr = addr;
  out_buf_rgn.size = size / 2;
  out_buf_rgn.curr_index = 0;

  inp_buf_rgn.addr = addr + size / 2;
  inp_buf_rgn.size = size / 2;
  inp_buf_rgn.curr_index = 0;

  if (!(selected_mode & INP)) {
    // No input, use the extra memory for output.

    out_buf_rgn.size = size;
  } else if (!(selected_mode & OUT)) {
    // No output, use the extra memory for input.

    inp_buf_rgn.addr = addr;
    inp_buf_rgn.size = size;
  }

  memset(addr, 0, size);
}

int memlog_getc() {
  if (!(selected_mode & INP)) {
    return -1;
  }

  if (inp_buf_rgn.addr[inp_buf_rgn.curr_index] == 0) {
    return -1;
  }

  int val = inp_buf_rgn.addr[inp_buf_rgn.curr_index];
  inp_buf_rgn.addr[inp_buf_rgn.curr_index] = 0;
  inp_buf_rgn.curr_index++;

  if (inp_buf_rgn.curr_index == inp_buf_rgn.size) {
    inp_buf_rgn.curr_index = 0;
  }

  return val;
}

void memlog_putc(char c) {
  if (!(selected_mode & OUT) || c == 0) {
    return;
  }

  out_buf_rgn.addr[out_buf_rgn.curr_index] = (uint8_t) c;
  out_buf_rgn.curr_index++;

  if (out_buf_rgn.curr_index == out_buf_rgn.size) {
    out_buf_rgn.curr_index = 0;
  }
}
