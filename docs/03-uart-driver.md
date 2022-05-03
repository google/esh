<!--

Copyright 2022 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

-->

# UART driver

Assuming that you have followed the steps listed in [platform_init and uart driver](02-platform_init-and-uart-driver.md), this section covers the implementation of the UART driver within `uart.c`. At this point the new project's directory may look as follows:

```
new_project
|-- Makefile
|-- platform.c
└──  uart
    |-- uart.c
    └── uart.h
```

`uart.c` must be looking as follows -

```
/**
 * Copyright <year> <author>
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

void uart_init(void) {

}

void uart_putc(char c) {

}

int uart_getc(void) {
  return -1;
}
```

For the shell to be functional we would need to implement the empty functions. The implementation of these functions may defer based on the type of uart.

## Constraints

- The driver has to be polling based and only allow reading and writing `1 Byte!`.
- UART interrupts should be disabled.
  - This is because the shell is to be used for quick enablement of the board.
  - Enabling and handling interrupts adds unnecessary complexity.
- Disable UART Tx/RX FIFO.
  - Handling FIFOs and sending/receiving multiple bytes adds to the driver complexity.
  - Keep the FIFO depth to 1.
- As part of the `uart_init()` be sure to enable the `tx` and `rx`.

## Example

For the `PL110` UART, the implementation looks as follows assuming the `uart` base is `0x00C00FEE` -
- `uart.h` must be looking as follows -

```
/**
 * Copyright <year> <author>
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

#ifndef __UART_H__
#define __UART_H__

#define UART_BASE 0x00C00FEE

void uart_init(void);
void uart_putc(char c);
int uart_getc(void);

#endif
```

- And `uart.c` must be looking as follows -

```
/**
 * Copyright <year> <author>
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

#define UART_DR *((unsigned int *)(UART_BASE + 0x00))
#define UART_TFR *((unsigned int *)(UART_BASE + 0x18))
#define UART_CR *((unsigned int *)(UART_BASE + 0x30))
#define UART_IFLS *((unsigned int *)(UART_BASE + 0x34))
#define UART_ICR *((unsigned int *)(UART_BASE + 0x44))

void uart_putc(char c) {
  while (UART_TFR & (1 << 5))
    ;

  UART_DR = (unsigned int)(c);
}

int uart_getc(void) {
  if ((UART_TFR & (1 << 4)) == 0) return UART_DR;

  return -1;
}

void uart_init(void) {
  UART_CR = (1 << 8) | (1 << 0);
  UART_ICR = 0x3ff;
  UART_IFLS = 0;
  UART_CR |= (1 << 9);
}
```

## Debugging tips

After the uart driver is implemented and is accurate, the compiled binary can be flashed on the board and the shell should be seen on the serial terminal connecting to the board's uart. If you don't see it, here are few things to consider:
- Ensure the `tx`/`rx` are enabled for the uart.
- Ensure hardware flow control is disabled. Check this both -
  - In the `uart_init()`, ensure for the particular uart the hardware flow control settings are disabled.
  - On the host machine, the serial application program should have have the hardware flow control disabled.
- Ensure, UART interrupts are disabled as part of the `uart_init()`.
- Check if the baud rate is set correctly and the serial application on the host is using the same baud to connect with the board.
- Check if the UART FIFO is disabled.
- Check if the UART IP is rightly clocked.
