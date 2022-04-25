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
#include "platform.h"
#include "mhu.h"

extern uint32_t vector_table_core1[];
extern volatile uint32_t g_mhu_flag0;
extern volatile uint32_t g_mhu_flag1;

void my_sleep(int secs) {
  #define STEPS_PER_SLEEP 65000000
  unsigned int i,s;
  for (s=0; s < secs; s++) {
    for (i=0; i < STEPS_PER_SLEEP; i++) {
       // skip CPU cycle for making loop
       // untouched by C compiler code optimizations
       asm("nop");
    }
  }
}

/**
 * @brief Inilialize the platform
 *
 */
void platform_init(){

    uart_init();
    set_read_char(ugetc);
    set_write_char(uputc);

    setup();
}


/**
 * @brief Get CPU ID of a core
 */
uint32_t get_cpu_id(){
    volatile uint32_t id =(uint32_t) *(volatile uint32_t*)CPU_ID_UNIT_BASE_SECURE;
    return id;
}

/**
 * @brief Check if the running core owning the Shell. Return try if shell is owned.
 */
uint32_t check_active_core() {
    volatile uint32_t id = get_cpu_id();
    if (id == CPU0_ID) {
        if (g_mhu_flag0 == 1) {
            return 1;
        }
        return 0;
    } else if (id == CPU1_ID) {
        if (g_mhu_flag1 == 1) {
            return 1;
        }
        return 0;
    }

    return 1;
}

/**
 * @brief print the core ID on the shell
 */
void print_core_id() {
  volatile uint32_t id = get_cpu_id();
  printf("[%d] ", id);
}

//overriding some of the shell methods below

void prepend_prompt()
{
    print_core_id();
}

int active_prompt()
{
    return check_active_core();
}

void setup(void)
{
    //get core-1 out of reset
    volatile uint32_t id = get_cpu_id();
    if (id == CPU0_ID) {
        //Step-1: set address of init vector for core-1
        //Step-2: Clear CPUWAIT bit for core-1
        struct sysctrl_t *sysctrl = (struct sysctrl_t *)CMSDK_SYSCTRL_BASE_S;
        sysctrl->initsvtor1 = (volatile uint32_t)vector_table_core1;
        sysctrl->cpuwait = 0x00;

        //wait for core-1 to get booted properly
        my_sleep(1);

        //give ownership of the shell to core-0
        g_mhu_flag0 = 1;
    }

    __asm volatile ("cpsid i" : : : "memory");    //disable interrupts
    mhu_init();
    __asm volatile ("cpsie i" : : : "memory");    //enable interrupts
}

