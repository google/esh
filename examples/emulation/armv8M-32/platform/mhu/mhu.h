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
#ifndef __ARMV8M_32_MHU_H__
#define __ARMV8M_32_MHU_H__

#include <stdint.h>

/* SSE 200 MHU register map structure */
typedef struct {
    /* (R/ ) CPU 0 Interrupt Status Register */
    volatile uint32_t cpu0_interrupt_status;
    volatile uint32_t cpu0_interrupt_set;  /* ( /W) CPU 0 Interrupt Set Register */
    volatile uint32_t cpu0_interrupt_clr;  /* ( /W) CPU 0 Interrupt Clear Register */
    volatile uint32_t reserved0;
    /* (R/ ) CPU 1 Interrupt Status Register */
    volatile uint32_t cpu1_interrupt_status;
    volatile uint32_t cpu1_interrupt_set;  /* ( /W) CPU 1 Interrupt Set Register */
    volatile uint32_t cpu1_interrupt_clr;  /* ( /W) CPU 1 Interrupt Clear Register */
    volatile uint32_t reserved1[1004];
    volatile uint32_t pidr4;         /* ( /W) Peripheral ID 4 */
    volatile uint32_t reserved2[3];
    volatile uint32_t pidr0;         /* ( /W) Peripheral ID 0 */
    volatile uint32_t pidr1;         /* ( /W) Peripheral ID 1 */
    volatile uint32_t pidr2;         /* ( /W) Peripheral ID 2 */
    volatile uint32_t pidr3;         /* ( /W) Peripheral ID 3 */
    volatile uint32_t cidr0;         /* ( /W) Component ID 0 */
    volatile uint32_t cidr1;         /* ( /W) Component ID 1 */
    volatile uint32_t cidr2;         /* ( /W) Component ID 2 */
    volatile uint32_t cidr3;         /* ( /W) Component ID 3 */
}MHU_TypeDef;

#define MHU0_BASE ((MHU_TypeDef *) 0x50003000)
#define MHU1_BASE ((MHU_TypeDef *) 0x50004000)
#define MHU0_IRQn 6
#define MHU1_IRQn 7

//driver APIs
void mhu_init();
void mhu_clear_val(MHU_TypeDef *base, uint32_t cpu_id);
void mhu_send(MHU_TypeDef *base, uint32_t cpu_id);
uint32_t mhu_get_status(MHU_TypeDef *base, uint32_t cpu_id);

//ISR
void mhu0_isr_core0();
void mhu0_isr_core1();
void mhu1_isr_core0();
void mhu1_isr_core1();

#endif //__ARMV8M_32_MHU_H__
