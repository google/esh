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

#include "string.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

int strcmp(const char *str1, const char *str2) {
  while (*str1 && (*str1 == *str2)) {
    str1++;
    str2++;
  }
  return *str1 - *str2;
}

char *strcpy(char *str1, const char *str2) {
  while (*str2) {
    *(str1++) = *(str2++);
  }
  *str1 = '\0';
  return 0;
}

void *memcpy(void *dest, const void *src, size_t size) {
  size_t i;
  char *src_char = (char *)src;
  char *dest_char = (char *)dest;
  for (i = 0; i < size; i++) {
    dest_char[i] = src_char[i];
  }
  return 0;
}

void *memset(void *dest, int val, size_t size) {
  size_t i;
  char *dest_char = (char *)dest;
  for (i = 0; i < size; i++) {
    dest_char[i] = val;
  }

  return 0;
}
