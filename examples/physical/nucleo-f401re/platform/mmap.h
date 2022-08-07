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

#ifndef __MMAP_H__
#define __MMAP_H__

#define USART_BASE    0x40004400
#define USART_SR      *((volatile unsigned int *)(USART_BASE + 0x00u))
#define USART_DR      *((volatile unsigned int *)(USART_BASE + 0x04u))
#define USART_BRR     *((volatile unsigned int *)(USART_BASE + 0x08u))
#define USART_CR1     *((volatile unsigned int *)(USART_BASE + 0x0cu))
#define USART_CR2     *((volatile unsigned int *)(USART_BASE + 0x10u))
#define USART_CR3     *((volatile unsigned int *)(USART_BASE + 0x14u))
#define USART_GTPR    *((volatile unsigned int *)(USART_BASE + 0x18u))

#define RCC_BASE      0x40023800
#define RCC_CR        *((volatile unsigned int *)(RCC_BASE + 0x00u))
#define RCC_PLLCFGR   *((volatile unsigned int *)(RCC_BASE + 0x04u))
#define RCC_CFGR      *((volatile unsigned int *)(RCC_BASE + 0x08u))
#define RCC_CIR       *((volatile unsigned int *)(RCC_BASE + 0x0Cu))
#define RCC_AHB1RSTR  *((volatile unsigned int *)(RCC_BASE + 0x10u))
#define RCC_AHB2RSTR  *((volatile unsigned int *)(RCC_BASE + 0x14u))
#define RCC_APB1RSTR  *((volatile unsigned int *)(RCC_BASE + 0x20u))
#define RCC_APB2RSTR  *((volatile unsigned int *)(RCC_BASE + 0x24u))
#define RCC_AHB1ENR   *((volatile unsigned int *)(RCC_BASE + 0x30u))
#define RCC_AHB2ENR   *((volatile unsigned int *)(RCC_BASE + 0x34u))
#define RCC_APB1ENR   *((volatile unsigned int *)(RCC_BASE + 0x40u))
#define RCC_APB2ENR   *((volatile unsigned int *)(RCC_BASE + 0x44u))
#define RCC_AHB1L     *((volatile unsigned int *)(RCC_BASE + 0x50u))
#define RCC_AHB2L     *((volatile unsigned int *)(RCC_BASE + 0x54u))
#define RCC_APB1L     *((volatile unsigned int *)(RCC_BASE + 0x60u))
#define RCC_APB2L     *((volatile unsigned int *)(RCC_BASE + 0x64u))
#define RCC_BDCR      *((volatile unsigned int *)(RCC_BASE + 0x70u))
#define RCC_CSR       *((volatile unsigned int *)(RCC_BASE + 0x74u))
#define RCC_SSCGR     *((volatile unsigned int *)(RCC_BASE + 0x80u))
#define RCC_PLLI2S    *((volatile unsigned int *)(RCC_BASE + 0x84u))

#define GPIOA_BASE    0x40020000
#define GPIOA_MODER   *((volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER  *((volatile unsigned int *)(GPIOA_BASE + 0x04))
#define GPIOA_OSPEEDR *((volatile unsigned int *)(GPIOA_BASE + 0x08))
#define GPIOA_PUPDR   *((volatile unsigned int *)(GPIOA_BASE + 0x0C))
#define GPIOA_IDR     *((volatile unsigned int *)(GPIOA_BASE + 0x10))
#define GPIOA_ODR     *((volatile unsigned int *)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR    *((volatile unsigned int *)(GPIOA_BASE + 0x18))
#define GPIOA_LCKR    *((volatile unsigned int *)(GPIOA_BASE + 0x1C))
#define GPIOA_AFRL    *((volatile unsigned int *)(GPIOA_BASE + 0x20))
#define GPIOA_AFRH    *((volatile unsigned int *)(GPIOA_BASE + 0x24))

#endif
