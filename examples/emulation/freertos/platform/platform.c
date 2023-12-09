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
#include <stdio.h>
#include "FreeRTOS.h"
#include "/home/yash/test/FreeRTOS-Kernel/include/task.h"
#include "/home/yash/test/FreeRTOS-Kernel/include/timers.h"
#include "queue.h"
#include "shell.h"
#include <stdbool.h>

#include "shell_config.h"
#include "shell.h"
#include "uart.h"

static void vTaskRunShell(void *pvParameters);


void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                         StackType_t ** ppxTimerTaskStackBuffer,
                                         uint32_t * pulTimerTaskStackSize )
    {
        static StaticTask_t xTimerTaskTCB;
        static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

        *ppxTimerTaskTCBBuffer = &( xTimerTaskTCB );
        *ppxTimerTaskStackBuffer = &( uxTimerTaskStack[ 0 ] );
        *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
    }

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName ){
                                            while(1);
                                        }
void vApplicationTickHook( void ){
    printf("Interuppt");
    while(1);
}
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                            StackType_t ** ppxIdleTaskStackBuffer,
                                            uint32_t * pulIdleTaskStackSize )
        {
            static StaticTask_t xIdleTaskTCB;
            static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

            *ppxIdleTaskTCBBuffer = &( xIdleTaskTCB );
            *ppxIdleTaskStackBuffer = &( uxIdleTaskStack[ 0 ] );
            *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
        }
void vApplicationMallocFailedHook(){
    printf("Malloc Failde");
    while(1);
}
void vAssertCalled( const char *pcFileName, uint32_t ulLine )
{   
    while(1);
}


void shell_function(void)
{   
    printf("Shell Function");
    int y = xTaskCreate(vTaskRunShell, "ShellTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    if (y == pdPASS) {
        // Task created successfully.
        printf("Success");
        // You can use xTaskHandle to refer to this task in the future.
    } else {

        printf("Failure");
        // Task creation failed.
        // Handle the error.
    }
    vTaskStartScheduler();

    printf("Reached END");
}

static void vTaskRunShell(void *pvParameters)
{
    (void)pvParameters;

    printf("hello");


    shell_function();
}
void platform_init() {


  shell_function();
  uart_init();
  printf("Executing platform.c");
  set_read_char(ugetc);
  set_write_char(uputc);
}