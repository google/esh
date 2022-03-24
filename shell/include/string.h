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

#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

/**
 * @brief Finds the length of the string
 *
 * @param str - string
 */
size_t strlen(const char *str);

/**
 * @brief Compares two strings
 *
 * @param str1 - first string
 * @param str2 - second string
 */
int strcmp(const char *str1, const char *str2);

/**
 * @brief Copies second string to first string
 *
 * @param str1 - first string
 * @param str2 - second string
 */
char *strcpy(char *str1, const char *str2);

/**
 * @brief Copies second buffer to the first buffer
 *
 * @param dest - pointer to the destination buffer
 * @param src - pointer to the source buffer
 * @param size - number of bytes to be copied
 */
void *memcpy(void *dest, const void *src, size_t size);

/**
 * @brief Sets memory buffer to the given value
 *
 * @param dest - pointer to the memory buffer
 * @param val - Value to set. Only the first byte is considered.
 * @param size - number of bytes to set
 */
void *memset(void *dest, int val, size_t size);

#endif  // __STRING_H__