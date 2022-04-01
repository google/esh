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
#ifndef __ARMV8M_32_UART_H__
#define __ARMV8M_32_UART_H__

typedef struct {
    volatile unsigned long data;         //0x00
    volatile unsigned long state;        //0x04
    volatile unsigned long control;      //0x08
    volatile unsigned long interupt;     //0x0C
    volatile unsigned long bauddiv;      //0x24
} UART_TypeDef;

/* CTRL Register */
#define UART_CMSDK_TX_EN       (1ul << 0)
#define UART_CMSDK_RX_EN       (1ul << 1)
#define UART_CMSDK_TX_INTR_EN  (1ul << 2)
#define UART_CMSDK_RX_INTR_EN  (1ul << 3)

/* STATE Register */
#define UART_CMSDK_TX_BF  (1ul << 0)
#define UART_CMSDK_RX_BF  (1ul << 1)

/* INTSTATUS Register */
#define UART_CMSDK_TX_INTR  (1ul << 0)
#define UART_CMSDK_RX_INTR  (1ul << 1)

/* UART state definitions */
#define UART_CMSDK_INITIALIZED  (1ul << 0)

#define Uart0 ((UART_TypeDef *) 0x51303000)
#define Uart1 ((UART_TypeDef *) 0x51304000)
#define Uart2 ((UART_TypeDef *) 0x51305000)


/**
 * @brief Put a character on uart data buffer
 *
 * @param c byte to be sent out
 */
void uputc(char c);

/**
 * @brief Get a character from uart data buffer
 *
 * @return int byte read from the uart data register
 */
int ugetc(void);

/**
 * @brief Init the uart, set baud etc
 *
 */
void uart_init(void);

#endif //__ARMV8M_32_UART_H__
