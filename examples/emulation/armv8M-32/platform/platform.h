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

#ifndef __ARMV8M_32_PLATFORM_H__
#define __ARMV8M_32_PLATFORM_H__

#include <arm_acle.h>

/**********************************************************************
 * System control register definitions
 */

#define CMSDK_SYSCTRL_BASE_S    0x50021000

struct sysctrl_t {
    volatile uint32_t secdbgstat;     /* (R/ ) Secure Debug Configuration
                                       *       Status Register
                                       */
    volatile uint32_t secdbgset;      /* ( /W) Secure Debug Configuration
                                       *       Set Register
                                       */
    volatile uint32_t secdbgclr;      /* ( /W) Secure Debug Configuration
                                       *       Clear Register
                                       */
    volatile uint32_t scsecctrl;      /* (R/W) System Control Security
                                       *       Control Register
                                       */
    volatile uint32_t fclk_div;       /* (R/W) Fast Clock Divider
                                       *       Configuration Register
                                       */
    volatile uint32_t sysclk_div;     /* (R/W) System Clock Divider
                                       *       Configuration Register
                                       */
    volatile uint32_t clockforce;     /* (R/W) Clock Forces */
    volatile uint32_t reserved0[57];
    volatile uint32_t resetsyndrome;  /* (R/W) Reset syndrome */
    volatile uint32_t resetmask;      /* (R/W) Reset MASK */
    volatile uint32_t swreset;        /* ( /W) Software Reset */
    volatile uint32_t gretreg;        /* (R/W) General Purpose Retention
                                       *       Register
                                       */
    volatile uint32_t initsvtor0;     /* (R/W) Initial Secure Reset Vector
                                       *       Register For CPU 0
                                       */
    volatile uint32_t initsvtor1;     /* (R/W) Initial Secure Reset
                                       *       Vector Register For CPU 1
                                       */
    volatile uint32_t cpuwait;        /* (R/W) CPU Boot wait control
                                       *       after reset
                                       */
    volatile uint32_t nmi_enable;     /* (R/W) NMI Enable Register */
    volatile uint32_t wicctrl;        /* (R/W) CPU WIC Request and
                                       *       Acknowledgment
                                       */
    volatile uint32_t ewctrl;         /* (R/W) External Wakeup Control */
    volatile uint32_t reserved1[54];
    volatile uint32_t pdcm_pd_sys_sense;      /* (R/W) Power Control Dependency
                                               * Matrix PD_SYS
                                               * Power Domain Sensitivity.
                                               */
    volatile uint32_t reserved2[2];           /* Reserved */
    volatile uint32_t pdcm_pd_sram0_sense;    /* (R/W) Power Control Dependency
                                               * Matrix PD_SRAM0 Power
                                               * Domain Sensitivity.
                                               */
    volatile uint32_t pdcm_pd_sram1_sense;    /* (R/W) Power Control Dependency
                                               * Matrix PD_SRAM1 Power
                                               * Domain Sensitivity.
                                               */
    volatile uint32_t pdcm_pd_sram2_sense;    /* (R/W) Power Control Dependency
                                               * Matrix PD_SRAM2 Power
                                               * Domain Sensitivity.
                                               */
    volatile uint32_t pdcm_pd_sram3_sense;    /* (R/W) Power Control Dependency
                                               * Matrix PD_SRAM3 Power
                                               * Domain Sensitivity.
                                               */
    volatile uint32_t reserved3[877];         /* Reserved */
    volatile uint32_t pidr4;                  /* (R/ ) Peripheral ID 4 */
    volatile uint32_t reserved4[3];
    volatile uint32_t pidr0;                  /* (R/ ) Peripheral ID 0 */
    volatile uint32_t pidr1;                  /* (R/ ) Peripheral ID 1 */
    volatile uint32_t pidr2;                  /* (R/ ) Peripheral ID 2 */
    volatile uint32_t pidr3;                  /* (R/ ) Peripheral ID 3 */
    volatile uint32_t cidr0;                  /* (R/ ) Component ID 0 */
    volatile uint32_t cidr1;                  /* (R/ ) Component ID 1 */
    volatile uint32_t cidr2;                  /* (R/ ) Component ID 2 */
    volatile uint32_t cidr3;                  /* (R/ ) Component ID 3 */
};

/**********************************************************************
 * CPU Elements register information
 */
#define CPU_ID_UNIT_BASE_SECURE 0x5001F000
#define CPU0_ID 0
#define CPU1_ID 1

/**********************************************************************
 * NVIC register information
 */

#define SCS_BASE            (0xE000E000UL)                             /*!< System Control Space Base Address */
#define NVIC_BASE           (SCS_BASE +  0x0100UL)                     /*!< NVIC Base Address */

/**********************************************************************
 * Methods exposed outside
 */
uint32_t get_cpu_id();
void my_sleep(int secs);
uint32_t check_active_core();
void print_core_id();

//shell methods override
void prepend_prompt();
int active_prompt();
void setup(void);

#endif //__ARMV8M_32_PLATFORM_H__
