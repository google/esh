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

#include "shell.h"

#include <stdbool.h>

#include "string.h"

// Build and Versioning related info
#define TO_STR(x) #x
#define TOSTRING(x) TO_STR(x)
#define BUILD_USER TOSTRING(__BUILD_USER__)
#define BUILD_HOST TOSTRING(__BUILD_HOST__)
#define SHELL_VERSION TOSTRING(__SHELL_VERSION__)
#define USER_REPO_VERSION TOSTRING(__USER_REPO_VERSION__)

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

#define PROMPT "# "
#define LINE_BUFF_SIZE 32
#define MAX_ARG_COUNT (LINE_BUFF_SIZE / 2)
#define NUM_HISTORY_ENTRIES 2

volatile int (*__read_char__)(void);
volatile void (*__write_char__)(char c);

extern unsigned long int __CMD_TABLE_START__;
extern unsigned long int __AUTO_TABLE_START__;

static const cmd_t *table = (cmd_t *)&__CMD_TABLE_START__;
static const cmd_t *auto_load = (cmd_t *)&__AUTO_TABLE_START__;

/*
 * To reduce the shell size the history feature
 * is made optional. Skip history feature if
 * SHELL_NO_HISTORY is defined.
 */
#ifndef SHELL_NO_HISTORY
static volatile int total_num_commands = 0;
static volatile int curr_command_ptr = 0;
static char cmd_history[NUM_HISTORY_ENTRIES][LINE_BUFF_SIZE];
#endif

static volatile bool echo = ECHO_INIT_VALUE;  // Should be set in the Makefile

void set_read_char(int (*func)(void)) { __read_char__ = func; }

void set_write_char(void (*func)(char)) { __write_char__ = func; }

__attribute__((weak)) void setup(void) {
  // to be provided by the user
}

__attribute__((weak)) void loop(void) {
  // to be provided by the user
}

static void set_echo(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: echo <on/off>\n");
    return;
  }

  if (!strcmp(argv[1], "on")) {
    echo = true;
  } else {
    echo = false;
  }
}

static void delete(void) {
  __write_char__(BACK_SPACE);
  __write_char__(SPACE);
  __write_char__(BACK_SPACE);
}

static void clear_prompt(int char_count) {
  while (char_count) {
    delete ();
    char_count--;
  }
}

/*
 * To reduce the shell size the history feature
 * is made optional. Skip history feature if
 * SHELL_NO_HISTORY is defined.
 */
#ifndef SHELL_NO_HISTORY
static void handle_up_arrow(char *cmd_buff, int *char_count) {
  if (curr_command_ptr < total_num_commands - NUM_HISTORY_ENTRIES ||
      curr_command_ptr == 0) {
    printf("%s", cmd_buff);
    return;
  }

  memset(cmd_buff, 0, LINE_BUFF_SIZE);

  curr_command_ptr--;
  int index = (curr_command_ptr % NUM_HISTORY_ENTRIES);
  memcpy(cmd_buff, &cmd_history[index], LINE_BUFF_SIZE);
  *char_count = strlen(cmd_buff);

  printf("%s", cmd_buff);
}

static void handle_down_arrow(char *cmd_buff, int *char_count) {
  memset(cmd_buff, 0, LINE_BUFF_SIZE);
  *char_count = 0;
  if (curr_command_ptr == total_num_commands) return;

  curr_command_ptr++;
  int index = (curr_command_ptr % NUM_HISTORY_ENTRIES);
  memcpy(cmd_buff, &cmd_history[index], LINE_BUFF_SIZE);
  *char_count = strlen(cmd_buff);

  printf("%s", cmd_buff);
}

static void add_command_to_history(const char *cmd_str) {
  int index = total_num_commands % NUM_HISTORY_ENTRIES;
  memcpy(&cmd_history[index], cmd_str, LINE_BUFF_SIZE);
  total_num_commands++;
  curr_command_ptr = total_num_commands;
}
#endif  // SHELL_NO_HISTORY

static int parse_line(char **argv, char *line_buff, int argument_size) {
  int argc = 0;
  int pos = 0;
  int length = strlen(line_buff);

  while (pos <= length) {
    if (line_buff[pos] != '\t' && line_buff[pos] != SPACE &&
        line_buff[pos] != END_OF_LINE)
      argv[argc++] = &line_buff[pos];

    for (; line_buff[pos] != '\t' && line_buff[pos] != SPACE &&
           line_buff[pos] != END_OF_LINE;
         pos++)
      ;

    if (line_buff[pos] == '\t' || line_buff[pos] == SPACE)
      line_buff[pos] = END_OF_LINE;

    pos++;
  }

  return argc;
}

static void execute(int argc, char **argv) {
  int match_found = FALSE;

  for (int i = 0; table[i].command_name != NULL; i++) {
    if (strcmp(argv[0], table[i].command_name) == 0) {
      table[i].command(argc, &argv[0]);
      match_found = TRUE;
      break;
    }
  }

  if (match_found == FALSE)
    printf("\"%s\": command not found. Use \"help\" to list all command.\n",
           argv[0]);
}

static void shell(void) {
  int s, argc;
  int count = 0;
  int special_key = 0;
  char c;

  char line_buff[LINE_BUFF_SIZE];
  char *argv[MAX_ARG_COUNT];

  for (int i = 0; i < LINE_BUFF_SIZE; i++) line_buff[i] = 0;

  for (int i = 0; i < MAX_ARG_COUNT; i++) argv[i] = NULL;

  printf(PROMPT);

  while (TRUE) {
    s = __read_char__();
    if (s != -1) {
      c = (char)s;

      if (c == CARRIAGE_RETURN || c == NEW_LINE) {
        line_buff[count] = END_OF_LINE;
        __write_char__(NEW_LINE);
        break;
      }

      if (c == DELETE || c == BACK_SPACE) {
        if (!echo) {
          delete ();
          delete ();
        }

        // guard against the count going negative!
        if (count == 0) continue;

        count--;

        line_buff[count] = END_OF_LINE;
        delete ();
      } else if (c == ESCAPE) {
        special_key = 1;
        continue;
      } else if (c == SQUARE_BRACKET_OPEN && special_key == 1) {
        special_key = 2;
        continue;
      } else if ((c == 'C' || c == 'D') && special_key != 0) {
        /* Ignore left/right arrow keys */
        special_key = 0;
        continue;
      } else if ((c == 'A' || c == 'B') && special_key == 2) {
        if (!echo) {
          clear_prompt(count + 4);
        } else {
          clear_prompt(count);
        }
/*
 * To reduce the shell size the history feature
 * is made optional. Skip history feature if
 * SHELL_NO_HISTORY is defined.
 */
#ifndef SHELL_NO_HISTORY
        if (c == 'A') {
          handle_up_arrow(line_buff, &count);
        } else {
          handle_down_arrow(line_buff, &count);
        }
#endif  // SHELL_NO_HISTORY
        special_key = 0;
        continue;
      } else {
        line_buff[count] = c;
        count++;
      }
      if (echo) {
        __write_char__(c);
      }
    } else {
      loop();
    }
  }

/*
 * To reduce the shell size the history feature
 * is made optional. Skip history feature if
 * SHELL_NO_HISTORY is defined.
 */
#ifndef SHELL_NO_HISTORY
  add_command_to_history(line_buff);
#endif

  // parse the line_buff
  argc = parse_line(argv, line_buff, MAX_ARG_COUNT);

  // execute the parsed commands
  if (argc > 0) execute(argc, argv);
}

static void exec_auto_cmds(void) {
  for (int i = 0; auto_load[i].command_name != NULL; i++) {
    auto_load[i].command(0, NULL);
  }
}

static void build_info(int argc, char **argv) {
  printf("Build: [" SHELL_VERSION ":" USER_REPO_VERSION "] - ["
          BUILD_USER "@" BUILD_HOST "] - " __DATE__ " - " __TIME__ "\n");
}

void initial_setup(void) {
  /**
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
  platform_init();
}

/**
 * @brief spwans the prompt
 *
 */
void prompt() {
  initial_setup();
  exec_auto_cmds();
  setup();

  while (TRUE) {
    shell();
  }
}

void exec(char *cmd_str) {
  int argc;

  // TODO: this takes too much stack space. Optimize!
  char *argv[MAX_ARG_COUNT];

  // parse the line_buff
  argc = parse_line(argv, cmd_str, MAX_ARG_COUNT);

  // execute the parsed commands
  if (argc > 0) execute(argc, argv);
}

void help(int argc, char **argv) {
  int i = 0;
  while (table[i].command_name != NULL) {
    printf(table[i].command_name);
    printf("\n\t");
    printf(table[i].command_help);
    printf("\n\n");
    i++;
  }
}

void printf_examples(int argc, char **argv) {
  printf("Printing printf examples\n");
  printf("%c \n", 'A');
  printf("%s \n", "Test");
  printf("%u \n", (uint32_t)(-1));
  printf("%d \n", -1);
  printf("%x \n", 0xDEADBEEF);
  printf("%lu \n", (uint32_t)(-2));
  printf("%ld \n", -2);
  printf("%lx \n", ~0xDEADBEEF);
  printf("%llu \n", (1ll << 60));
  printf("%lld \n", (1ll << 63));
  printf("%llx \n", (0xDEADBEEFll << 32) | 0xDEADBEEF);
}

// DO NOT REMOVE THESE
AUTO_CMD(version, "Prints details of the build", build_info);
ADD_CMD(help, "Prints all available commands", help);
ADD_CMD(printf_examples, "Prints example usage of printf", printf_examples);
ADD_CMD(echo, "Turn input echo on/off", set_echo);

// Mandatory!
__attribute__((section(".cmd_end"))) cmd_t cmd_end_ = {NULL, NULL, NULL};
