/*
# Copyright 2022 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/
#include "multicore.h"

static cmd function_addr;  // holds address of function triggered

/**
 * Secondary Cores complete their boot up and jump to secondary core main
 */
static void secondary_core_main(uint64_t cpuid) {
    printf("Core %ld Bootup done\n", cpuid);
}
/**
 * start_core is called quickly after main core completes its boot.
 * start_core wakes up other secondary cores and calls them to specific function address
 */
void start_core(uint64_t cpuid, void (*func)(uint64_t)) {
    printf("Core 0 Starting Core %ld.\n", cpuid);
    uint64_t offset = WATCH_VALUE_SIZE * (cpuid);
    *(uint64_t *)(((uint64_t)&spin_cpu) + offset) = (uint64_t)func; // Save caller address
    asm volatile("sev"); // Since we put core 1 to sleep with a wfe (Wait For Event) instruction, we use a sev (Set Event) instruction to wake it again.
}
/**
 * Core 0 wakes up the secondary cores by updating a non-zero value at spin address.The non-zero value is the function address
 * where the secondary cores will start executing instruction after booting up.
 */
static int secondary_core_boot(int argc, char **argv) {
    uint64_t cpuid;
    for (cpuid = 1; cpuid < PLATFORM_CORE_COUNT; cpuid++) {
        start_core(cpuid, secondary_core_main);
    }
    printf("Core 0 Bootup done.\n"); // Core 0 Completes boot up
    return 0;
}

// test function to check multi-core execution
int test_fun1(int cpuid, char *argv[]) {
    for (int i = 0; i <= 10; i++) {
        printf("Core:%ld executing fun1\n", cpuid);
    }
    return 0;
}

// test function to check multi-core execution
int test_fun2(int cpuid, char *argv[]) {
    for (int i = 0; i <= 10; i++) {
        printf("Core:%ld executing fun2\n", cpuid);
    }
    return 0;
}

// Checks if given core id and function name are valid or not.
static int check_args(int cpuid, char *cmd_str) {
    // check core_id
    if (cpuid < 0 || cpuid >= PLATFORM_CORE_COUNT) {
        printf("Invalid Core Number\n");
        return 0;
    }
    // get function address
    function_addr = get_function_addr(cmd_str);
    // check function name
    if (function_addr == NULL) {
        printf("Invalid Function Name: %s\n", cmd_str);
        return 0;
    }
    return 1;
}

// Writes the triggered function address in the given core's watch address.
static void wrt_watch_addr(int cpuid) {
    printf("\nStarting Core %ld\n", cpuid);
    uint64_t offset = WATCH_VALUE_SIZE * (cpuid);
    *(uint64_t *)(((uint64_t)&spin_cpu) + offset) = (uint64_t)function_addr; // Save caller address
}

// Executes the given function on given core
static int function_execute(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <core_number> <function_name>\n", argv[0]);
        return -1;
    }
    int cpuid = atoi(argv[1]);

    if (check_args(cpuid, argv[2]) == 0) {
        printf("Usage: %s <core_number> <function_name>\n", argv[0]);
        return -1;
    }

    // given core is a secondary core
    if (cpuid != 0) {
        wrt_watch_addr(cpuid);
    }

    printf("\nStarting Execution....\n\n");
    asm volatile("sev");

    // given core is main core
    if (cpuid == 0) {
        function_addr(0, NULL);
    }

    // wait for executing secondary core to return
    volatile int i = 0;
    while (i < 1000000) {
        i++; //  delay of 1000000 cycles.
    }
    // return back to prompt
    return 0;
}

ADD_CMD(Function1, "Test function", test_fun1);
ADD_CMD(Function2, "Test function", test_fun2);
ADD_CMD(run_task, "Executes functions on given Core Number", function_execute);
AUTO_CMD(SecondaryBoot, "Starts Secondary Cores", secondary_core_boot);
