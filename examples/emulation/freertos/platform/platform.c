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
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "shell.h"
#include <stdbool.h>

#include "shell_config.h"
#include "string.h"
#include "shell.h"
#include "uart.h"

/**
 * @brief Inilialize the platform
 *
 */
static void vTaskRunShell(void *pvParameters);
extern void prompt();
// extern void vApplicationTickHook(void);
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    /* Check pcTaskName for the name of the offending task,
     * or pxCurrentTCB if pcTaskName has itself been corrupted. */
    ( void ) xTask;
    ( void ) pcTaskName;
}


void vApplicationMallocFailedHook()
{   
    // printf("Application Malloc Falied");
	// while(1);
}

void vApplicationTickHook(void) {
    
}
void vAssertCalled( const char *pcFileName, uint32_t ulLine ) {
//   printf("Assert! Spining forever!\n");
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
void main_blinky1(void)
{ 
    //   platform_init();
    prompt();
    xTaskCreate(vTaskRunShell, "ShellTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    vTaskStartScheduler();
}

static void vTaskRunShell(void *pvParameters)
{
    (void)pvParameters;

    // printf("hello");
	
	prompt();


}
void platform_init() {

  uart_init();
  set_read_char(ugetc);
  set_write_char(uputc);
}
int bootFreeRTOS(){
    xTaskCreate(vTaskRunShell, "ShellTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    vTaskStartScheduler();
    return 0;
}
ADD_CMD(bootFreeRTOS,"ABCD",bootFreeRTOS);