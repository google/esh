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

#ifndef __PRINTF_H__
#define __PRINTF_H__

// Currently, printf handles 64 bit integers, which can take a maximum value of
// 2^64 - 1 when the integer is unsigned, which is approximately 10^19.
// If the 64 bit support is disabled then 2^32-1 which is ~10^10
#ifndef SHELL_NO_PRINTF_LL
#define MAX_DIGITS 20
#else
#define MAX_DIGITS 10
#endif

// This is used while printing hexadecimal values.
#ifndef SHELL_NO_PRINTF_LL
#define LAST_NIBBLE_FIRST_BIT 60
#else
#define LAST_NIBBLE_FIRST_BIT 28
#endif
/**
 * @brief prints an unformatted string to the UART
 *
 * @param s string
 */
int printf(const char *s, ...);

#endif  // __PRINTF_H__
