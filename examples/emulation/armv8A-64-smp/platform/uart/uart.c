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

#include "shell.h"
#include "uart.h"

#define UART_DR *((unsigned int *)(UART_BASE + 0x00))
#define UART_TFR *((unsigned int *)(UART_BASE + 0x18))
#define UART_CR *((unsigned int *)(UART_BASE + 0x30))
#define UART_IFLS *((unsigned int *)(UART_BASE + 0x34))
#define UART_ICR *((unsigned int *)(UART_BASE + 0x44))

void uputc(char c) {
  while (UART_TFR & (1 << 5))
    ;

  UART_DR = (unsigned int)(c);
}

int ugetc(void) {
  if ((UART_TFR & (1 << 4)) == 0) return UART_DR;

  return -1;
}

void uart_init(void) {
  UART_CR = (1 << 8) | (1 << 0);
  UART_ICR = 0x3ff;
  UART_IFLS = 0;
  UART_CR |= (1 << 9);
}
