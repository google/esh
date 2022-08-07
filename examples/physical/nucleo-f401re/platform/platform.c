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

#include "mmap.h"
#include "shell.h"
#include "uart.h"

void set_gpio_af() {
  RCC_APB1ENR |= 1 << 17 | 1<<24;
  RCC_AHB1ENR |= 0x1;

  // USART2 Tx, Rx (PA2, PA3)  Alternate function mode
  GPIOA_MODER   |=  (0b10 << 4) | (0b10 << 6);

  // USART2 Tx (PA2) Output push-pull (reset state)
  GPIOA_OTYPER  &=  ~( 1 << 2);

  // USART2 Rx (PA3)  High speed
  GPIOA_OSPEEDR |=  (0b11 << 4);

  // USART2 Tx (PA2), RX (PA3) Alternate function 7 USART
  GPIOA_AFRL    |=  (0b0111 << 8) | (0b0111 << 12);
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
  set_gpio_af();
  uart_init();
  set_read_char(ugetc);
  set_write_char(uputc);
}
