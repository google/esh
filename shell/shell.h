/**
 * Copyright 2021 Google LLC
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

#ifndef __H_SHELL__
#define __H_SHELL__

#include <stdint.h>

#include "a2i.h"
#include "printf.h"
#include "regs.h"
#include "string.h"

/**
 * @brief Looks for and executes a string as a command
 *
 * @param cmd_str command string to be executed
 */
void exec(char *cmd_str);

/**
 * @brief Expected format of the user defined function
 *
 */
typedef void (*cmd)(int argc, char **argv);

/**
 * @brief Command table entry format
 *
 * THIS NOT SUPPOSED TO BE USED BY THE USER
 * DIRECTLY.
 *
 */
typedef struct {
  char *command_name;
  char *command_help;
  cmd command;
} cmd_t;

/**
 * @brief Format for the user to add a command
 *
 * @param _name Name of the command that should trigger the function
 *              on the shell
 * @param _help_string String describing the function/command
 * @param _function fucntion that needs to be made available in the shell
 *
 */
#define ADD_CMD(_name, _help_string, _function) \
  __attribute__((section(".cmd_list")))         \
  cmd_t _var##_function = {#_name, _help_string, &_function}

/**
 * @brief Format for the user to add a command to be executed quickly after boot
 *        This command will also be available from the console!
 *
 * @param _name Name of the command that should trigger the function
 *              on the shell
 * @param _help_string String describing the function/command
 * @param _function fucntion that needs to be made available in the shell
 *
 */
#define AUTO_CMD(_name, _help_string, _function) \
  __attribute__((section(".auto_list")))         \
  cmd_t _var##_function = {#_name, _help_string, &_function}

/**
 * @brief Set a way to read a byte from input source.
 *        The use needs to set a function that can be used to read a character
 *        from a source. This way the shell would be able to read incoming
 *        characters. A call to this function can be in the platform_init()
 *        function or during the runtime!
 *
 * @param func function pointer to a function that should be used to read a
 *        character.
 */
void set_read_char(int (*func)(void));

/**
 * @brief Set a way to write a byte to output source.
 *        The use needs to set a function that can be used to write a character
 *        to destination. This way the shell would be able to write outgoing
 *        characters. A call to this function can be in the platform_init()
 *        function or during the runtime!
 *
 * @param func function pointer to a function that should be used to write a
 *        character.
 */
void set_write_char(void (*func)(char));

/**
 * NOTE TO USER!!!
 *
 * platform_init() is expected to be defined by the user.
 * Expectation is that the user would do any needed hardware initialization
 * And register two functions to read a byte and write a byte using
 * - set_read_char(<way to read a byte>);
 * - set_write_char(<way to write a byte>);
 *
 * typical implementation may look like:
 * void platform_init(void) {
 *   uart_init();
 *   set_read_char(uart_getchar);
 *   set_write_char(uart_putchar);
 * }
 */
void platform_init(void);

#endif
