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

/**
 * @brief Default UART implementation in case the user hasn't provided
 *        putc(), get(), uart_init() implementation.
 *
 */
#define UART_DR *((unsigned int *)(UART_BASE_PHYSICAL + 0x00))
#define UART_STATE *((unsigned int *)(UART_BASE_PHYSICAL + 0x04))
#define UART_CTRL *((unsigned int *)(UART_BASE_PHYSICAL + 0x08))
#define UART_INT *((unsigned int *)(UART_BASE_PHYSICAL + 0x04))

void uputc(char c) {
  while (UART_STATE & 1)
    ;

  UART_DR = c;
}

int ugetc(void) {
  if (UART_STATE & (1 << 1)) return UART_DR;

  return -1;
}

void uart_init(void) {
  UART_CTRL = 0x0;
  UART_INT = 0xF;
  UART_CTRL = 0x3;
}
