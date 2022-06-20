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

static cmd cpu_function_addr[PLATFORM_CORE_COUNT];  // holds address of function
                                                    // triggered for each cpu

/**
 * Secondary Cores complete their boot up and jump to secondary core main
 */
static void secondary_core_main(uint64_t cpuid) {
  multicore_printf("Core %ld Bootup done\n", cpuid);
}
/**
 * start_core is called quickly after main core completes its boot.
 * start_core wakes up other secondary cores and calls them to specific function
 * address
 */
void start_core(uint64_t cpuid, void (*func)(uint64_t)) {
  multicore_printf("Core 0 Starting Core %ld.\n", cpuid);
  spin_cpu[cpuid] = (uint64_t)func;  // Save caller address
  asm volatile("sev");  // Since we put core 1 to sleep with a wfe (Wait For
                        // Event) instruction, we use a sev (Set Event)
                        // instruction to wake it again.
}
/**
 * Core 0 wakes up the secondary cores by updating a non-zero value at spin
 * address.The non-zero value is the function address where the secondary cores
 * will start executing instruction after booting up.
 */
static int secondary_core_boot(int argc, char **argv) {
  uint64_t cpuid;
  for (cpuid = 1; cpuid < PLATFORM_CORE_COUNT; cpuid++) {
    start_core(cpuid, secondary_core_main);
  }
  multicore_printf("Core 0 Bootup done.\n");  // Core 0 Completes boot up
  return 0;
}

// test function to check multi-core execution
int test_fun1(int cpuid, char *argv[]) {
  for (int i = 0; i <= 10; i++) {
    multicore_printf("Core:%ld executing fun1\n", cpuid);
  }
  return 0;
}

// test function to check multi-core execution
int test_fun2(int cpuid, char *argv[]) {
  for (int i = 0; i <= 10; i++) {
    multicore_printf("Core:%ld executing fun2\n", cpuid);
  }
  return 0;
}

// Checks if given core id and function name are valid or not.
static int check_args(int cpuid, char *cmd_str, int arg_num) {
  // check core_id
  if (cpuid < 0 || cpuid >= PLATFORM_CORE_COUNT) {
    multicore_printf("Argument %ld: Invalid Core Number\n", arg_num + 1);
    return 0;
  }
  // get and save function address
  cpu_function_addr[cpuid] = get_function_addr(cmd_str);
  // check function name
  if (cpu_function_addr[cpuid] == NULL) {
    multicore_printf("Argument %ld: Invalid Function Name: %s\n", arg_num + 2,
                     cmd_str);
    return 0;
  }
  return 1;
}

// Writes the triggered function address in the given core's watch address.
static void wrt_watch_addr(int cpuid) {
  multicore_printf("\nStarting Core %ld\n", cpuid);
  spin_cpu[cpuid] = (uint64_t)cpu_function_addr[cpuid];  // Save caller address
}

// Wait untill all cores are avilable
static void wait_for_cores(void) {
  // primary core loops for all platform cores
  for (int cpuid = 1; cpuid < PLATFORM_CORE_COUNT; cpuid++) {
    uint64_t value = core_available[cpuid];
    // loop untill core is avilable
    while (value != 1) {
      multicore_printf("Waiting for Core %ld\n", cpuid);
      value = core_available[cpuid];
    }
    multicore_printf("Core %ld Available\n", cpuid);
  }
}

// Runs the different functions in parallel on multiple core
static int function_execute(int argc, char *argv[]) {
  if (argc < 3 || argc % 2 == 0) {
    multicore_printf("Usage: %s <core_number> <function_name>\n", argv[0]);
    return -1;
  }
  // initialize core 0's func address to remove prev value
  cpu_function_addr[0] = NULL;

  // Check each argument is valid or not
  for (int i = 1; i < argc; i += 2) {
    int cpuid = atoi(argv[i]);
    if (check_args(cpuid, argv[i + 1], i) == 0) {
      multicore_printf("Usage: %s <core_number> <function_name>\n", argv[0]);
      return -1;
    }
  }

  // save function address in watch address
  for (int i = 1; i < argc; i += 2) {
    int cpuid = atoi(argv[i]);
    // given core is a secondary core
    if (cpuid != 0) {
      wrt_watch_addr(cpuid);
    }
  }

  multicore_printf("\nStarting Execution....\n\n");
  asm volatile("sev");

  // if there is a task to be executed on primary core
  if (cpu_function_addr[0] != NULL) {
    cpu_function_addr[0](0, NULL);
  }

  // before returning to prompt,
  // primary core waits till all secondary cores complete execution and are
  // avilable
  wait_for_cores();
  return 0;
}

// Runs a function in parallel on each platform core
static int execute_on_each_cpu(int argc, char *argv[]) {
  if (argc < 2) {
    multicore_printf("Usage: %s <function_name>\n", argv[0]);
    return -1;
  }
  // holds address of function to be executed on each core
  cmd func = get_function_addr(argv[1]);
  // Check if function is valid or not
  if (func == NULL) {
    multicore_printf("Invalid Function Name: %s\n", argv[1]);
    multicore_printf("Usage: %s <function_name>\n", argv[0]);
    return -1;
  }

  for (int cpuid = 1; cpuid < PLATFORM_CORE_COUNT; cpuid++) {
    cpu_function_addr[cpuid] = func;  // each core runs same function
    wrt_watch_addr(cpuid);
  }

  multicore_printf("\nStarting Execution....\n\n");
  asm volatile("sev");

  // task executed on primary core
  func(0, NULL);
  // before returning to prompt,
  // primary core waits till all secondary cores complete execution and are
  // avilable
  wait_for_cores();
  return 0;
}

ADD_CMD(Function1, "Test function", test_fun1);
ADD_CMD(Function2, "Test function", test_fun2);
ADD_CMD(run_task, "Deploy functions on given cores", function_execute);
ADD_CMD(run_all, "Deploy function on all cores", execute_on_each_cpu);
AUTO_CMD(SecondaryBoot, "Starts Secondary Cores", secondary_core_boot);