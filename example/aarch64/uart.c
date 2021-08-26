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
#define UART_DR    *((unsigned int *) (UART_BASE_PHYSICAL + 0x00))
#define UART_TFR   *((unsigned int *) (UART_BASE_PHYSICAL + 0x18))
#define UART_CR    *((unsigned int *) (UART_BASE_PHYSICAL + 0x30))
#define UART_IFLS  *((unsigned int *) (UART_BASE_PHYSICAL + 0x34))
#define UART_ICR   *((unsigned int *) (UART_BASE_PHYSICAL + 0x44))

void putc(char c) {
    while (UART_TFR & (1<<5));

    UART_DR = (unsigned int)(c);
}

int getc(void) {
    if ((UART_TFR & (1 << 4)) == 0)
        return UART_DR;

    return -1;
}

void uart_init(void) {
    UART_CR = (1<<8)|(1<<0);
    UART_ICR = 0x3ff;
    UART_IFLS = 0;
    UART_CR |= (1<<9);
}