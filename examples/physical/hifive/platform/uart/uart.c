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

#include <stdint.h>
#include "uart.h"

#define BAUD_115200 139
#define UART0_TX 16
#define UART0_RX 17

#define GPIO_BASE (0x10012000UL)

#define UART_TXDATA (*((volatile uint32_t *)(UART_BASE + 0x0)))
#define UART_RXDATA (*((volatile uint32_t *)(UART_BASE + 0x4)))
#define UART_DIV (*((volatile uint32_t *)(UART_BASE + 0x18)))

#define GPIO_IOF_EN (*((uint32_t *)(GPIO_BASE + 0x38)))
#define GPIO_IOF_SEL (*((uint32_t *)(GPIO_BASE + 0x3c)))

void uart_init() {
  UART_DIV = BAUD_115200;
  GPIO_IOF_EN |= (1u << UART0_TX) | (1u << UART0_RX);
  GPIO_IOF_SEL &= ~((1u << UART0_TX) | (1u << UART0_RX));
}

void putc(char c) {
  while (UART_TXDATA & (1u << 31))
    ;
  UART_TXDATA = c;

  if (c == '\n') {
    while (UART_TXDATA & (1u << 31))
      ;
    UART_TXDATA = '\r';
  }
}

int getc(void) {
  while (1) {
    uint32_t c = UART_RXDATA;
    if (!(c & (1u << 31))) return (int)c;
  }
}
