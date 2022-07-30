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

/**
 * @brief Default UART implementation in case the user hasn't provided
 *        uputc(), uget(), uart_init() implementation.
 *
 */
#include "uart.h"

void uart_init(void) {
    Uart0->control = UART_CMSDK_RX_EN | UART_CMSDK_TX_EN;
}

void uputc(char ch)
{
    while (Uart0->state & UART_CMSDK_TX_BF); //busy wait
    Uart0->data = ch;
}

int ugetc(void)
{
    while (!(Uart0->state & UART_CMSDK_RX_BF)); //wait for data
    return Uart0->data;
}

