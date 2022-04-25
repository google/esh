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
 * @brief Very basic MHU (Message handling Unit) driver for SSE-200
 */
#include "mhu.h"
#include "platform.h"

volatile uint32_t g_mhu_flag0 = 0;    //shell ownership flag for core-0
volatile uint32_t g_mhu_flag1 = 0;    //shell ownership flag for core-1

/*
 * @brief Check MHU status register
 */
uint32_t mhu_get_status(MHU_TypeDef *base, uint32_t cpu_id)
{
    uint32_t status;
    if (cpu_id == CPU0_ID) {
        status = base->cpu0_interrupt_status;
    } else if (cpu_id == CPU1_ID) {
        status = base->cpu1_interrupt_status;
    } else {
        while(1);
    }

    return status;
}

/*
 * @brief set MHU interrupt flag
 */
void mhu_send(MHU_TypeDef *base, uint32_t cpu_id)
{
    const uint32_t set_val = 0x01;
    if (cpu_id == CPU0_ID) {
        base->cpu0_interrupt_set = set_val;
    } else if (cpu_id == CPU1_ID) {
        base->cpu1_interrupt_set = set_val;
    } else {
        while(1);
    }
}

/*
 * @brief Clear MHU interrupt flag
 */
void mhu_clear_val(MHU_TypeDef *base, uint32_t cpu_id)
{
    const uint32_t clr_val = 0x01;
    if (cpu_id == CPU0_ID) {
        base->cpu0_interrupt_clr = clr_val;
    } else if (cpu_id == CPU1_ID) {
        base->cpu1_interrupt_clr = clr_val;
    } else {
        while(1);
    }
}

/*
 * @brief Initialize MHU - mainly interrupts are enabled
 */

void mhu_init()
{
    //enable MHU interrupts
    uint32_t mask = 1<<MHU0_IRQn;
    volatile uint32_t *ibase = (volatile uint32_t *) NVIC_BASE;
    uint32_t val = *ibase;
    val = val | mask;
    mask = 1 << MHU1_IRQn;
    val = val | mask;
    *ibase = val;
}


/*
 * @brief MHU0 core0 ISR
 */
void mhu0_isr_core0(void)
{
    mhu_clear_val(MHU0_BASE, CPU0_ID);
    if (g_mhu_flag0 == 0) {
        g_mhu_flag0 = 1;
    } else {
        ;
    }
}

/*
 * @brief MHU0 core1 ISR
 */
void mhu0_isr_core1(void)
{
    mhu_clear_val(MHU0_BASE, CPU1_ID);
    if (g_mhu_flag1 == 0) {
        g_mhu_flag1 = 1;
    } else {
        ;
    }
}

/*
 * @brief MHU1 core0 ISR
 */
void mhu1_isr_core0(void)
{
    mhu_clear_val(MHU1_BASE, CPU0_ID);
    if (g_mhu_flag0 == 0) {
        g_mhu_flag0 = 1;
    } else {
        ;
    }
}

/*
 * @brief MHU1 core1 ISR
 */
void mhu1_isr_core1(void)
{
    mhu_clear_val(MHU1_BASE, CPU1_ID);
    if (g_mhu_flag1 == 0) {
        g_mhu_flag1 = 1;
    } else {
        ;
    }
}
