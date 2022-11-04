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

#ifndef __H_SHELL_CONFIG__
#define __H_SHELL_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Build information related macros
 */
#define TO_STR(x) #x
#define TOSTRING(x) TO_STR(x)

/**
 * Who triggered the build.
 */
#define BUILD_USER TOSTRING(__BUILD_USER__)

/**
 * Hostname of the machine this was built on.
 */
#define BUILD_HOST TOSTRING(__BUILD_HOST__)

/**
 * latest git commit sha for shell.
 */
#define SHELL_VERSION TOSTRING(__SHELL_VERSION__)

/**
 * git commit sha for user's repo.
 */
#define USER_REPO_VERSION TOSTRING(__USER_REPO_VERSION__)

/**
 * Prompt
 */
#define USER_PROMPT TOSTRING(__PROMPT__)
#define PROMPT USER_PROMPT " "

/**
 * Max characters on the prompt that the shell will scan and process.
 * Defaults to 32.
 */
#ifndef LINE_BUFF_SIZE
#define LINE_BUFF_SIZE 32
#endif

/**
 * Maximum arguments can be half of line buffer.
 */
#define MAX_ARG_COUNT (LINE_BUFF_SIZE / 2)

/**
 * Maximum commands to be remembered in the history buffer.
 */
//define if user has not defined NUM_HISTORY_ENTRIES
#ifndef NUM_HISTORY_ENTRIES
#define NUM_HISTORY_ENTRIES 2
#endif


#define NULL ((void *)0)
#define TRUE (1 == 1)
#define FALSE (1 == 0)

// Key codes
#define END_OF_LINE '\0'
#define SPACE ' '
#define TAB '\t'
#define NEW_LINE '\n'
#define CARRIAGE_RETURN '\r'
#define BACK_SPACE '\b'
#define DELETE '\177'
#define ESCAPE '\33'
#define SQUARE_BRACKET_OPEN '\133'
#define UP_ARROW '\101'

#ifdef __cplusplus
}
#endif

#endif
