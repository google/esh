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

#include "uart.h"

void uputc(char c) {
  while ((UARTFR >> 5) & 1)
    ;

  UART_DR = c;
  if (c == '\n')
    uputc('\r');
}

int ugetc(void) {
  if (!((UARTFR >> 4) & 1)) return UART_DR;

  return -1;
}

void uart_init(void) {
  /* Disable UART0 */
  UARTCTL = 0;

  /* Set the Baud to 115200 */
  UARTIBRD = 8;
  UARTFBRD = 44;

  /* Use system clock for Uart */
  UARTCC = 0;

  /* 8n1 and no fifo */
  UARTLCRH = 0x60;

  /* Enable UART0, TXE and RXE */
  UARTCTL = 0x301;
}
