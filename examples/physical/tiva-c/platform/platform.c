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

#define RCGCGPIO *((unsigned int *)(0x400FE000 + 0x608))
#define RCGCUART *((unsigned int *)(0x400FE000 + 0x618))

#define GPIO_PORT_A_BASE 0x40004000
#define GPIODEN *((unsigned int *) (GPIO_PORT_A_BASE + 0x51C))
#define GPIOAFSEL *((unsigned int *) (GPIO_PORT_A_BASE + 0x420))
#define GPIOPCTL *((unsigned int *) (GPIO_PORT_A_BASE + 0x52C))

void pin_setup() {
  /* Enable clock for UART0 and GPIO */
  RCGCUART |= 1;
  RCGCGPIO |= 1;

  /* Set PIN PA0 and PA1 as uart */
  GPIODEN = 0x03;
  GPIOAFSEL = 0x03;
  GPIOPCTL |= 0x00000011;
}

void setup_bss_data(){
  extern unsigned long int _etext, _data, _edata, _bss, _ebss;
  unsigned long int *src = &_etext, *dst = &_data;

  /* init .data section */
  while(dst < &_edata)
    *(dst++) = *(src++);

  /* Clear .bss*/
  for(dst = &_bss; dst < &_ebss; dst++)
    *dst = 0;
}

/**
 * @brief Inilialize the platform
 *
 */
void platform_init() {
    setup_bss_data();
    pin_setup();
    uart_init();
    set_read_char(ugetc);
    set_write_char(uputc);
}
