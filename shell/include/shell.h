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

#include "printf.h"
#include "string.h"

#include <stdint.h>

/**
 * @brief Expected format of the user defined function
 *
 */
typedef void (*cmd)(int argc, char** argv);

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
    __attribute__((section(".cmd_list"))) cmd_t _var##_name = {#_name, _help_string, &_function}

#endif
