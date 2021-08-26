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

#define NULL              ((void *)0)
#define TRUE              (1==1)
#define FALSE             (1==0)

// Key codes
#define END_OF_LINE       '\0'
#define SPACE             ' '
#define TAB               `\t`
#define NEW_LINE           '\n'
#define CARRIAGE_RETURN   '\r'
#define BACK_SPACE        '\b'
#define DELETE            '\177'

#define PROMPT            "# "
#define LINE_BUFF_SIZE    64
#define MAX_ARG_COUNT     (LINE_BUFF_SIZE / 2)

extern int getc(void);
extern void putc(char c);

extern unsigned long int __CMD_TABLE_START__;
static cmd_t *table = (cmd_t *)&__CMD_TABLE_START__;

void printf(const char *s) {
    while(*s != END_OF_LINE) {
        putc(*s);
        s++;
    }
}

static int strcmp(char *s1, char *s2) {
    while (*s1 == *s2) {
        if (*s1 == END_OF_LINE && *s2 == END_OF_LINE)
            return 0;

        if (*s1 != *s2 && ( *s1 == END_OF_LINE || *s2 == END_OF_LINE))
            return -1;

        s1++;
        s2++;
    }

    return -2;
}

static int strlen(char *s) {
    int len = 0;

    while (*s++ != END_OF_LINE)
        len++;

    return len;
}

static int parse_line(char** argv, char *line_buff, int argument_size) {
    int argc = 0;
    int pos  = 0;
    int length = strlen(line_buff);

    while (pos <= length) {
        if (line_buff[pos] != '\t' && line_buff[pos] != SPACE && line_buff[pos] != END_OF_LINE)
            argv[argc++] = &line_buff[pos];

        for (; line_buff[pos] != '\t' && line_buff[pos] != SPACE && line_buff[pos] != END_OF_LINE; pos++);

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
            table[i].command (argc, &argv[0]);
            match_found = TRUE;
            break;
        }
    }

    if (match_found == FALSE)
        printf("command not found!\n");
}

static void delete(void) {
    putc(BACK_SPACE);
    putc(SPACE);
    putc(BACK_SPACE);
}

static void shell(void) {
    int s, argc;
    int count = 0;
    char c;

    char line_buff[LINE_BUFF_SIZE];
    char *argv[MAX_ARG_COUNT];

    for (int i = 0; i < LINE_BUFF_SIZE; i++)
        line_buff[i] = 0;

    for (int i = 0; i < MAX_ARG_COUNT; i++)
        argv[i] = NULL;

    printf(PROMPT);

    while (TRUE) {
        s = getc();
        if (s != -1) {
            c = (char)s;

            if (c == CARRIAGE_RETURN || c == NEW_LINE) {
                line_buff[count] = END_OF_LINE;
                putc(NEW_LINE);
                break;
            }

            if (c == DELETE) {
                // guard against the count going negative!
                if (count == 0)
                    continue;

                count--;

                line_buff[count] = END_OF_LINE;
                delete ();
            }
            else
            {
                line_buff[count] = c;
                count++;
            }
            putc(c);
        }
    }

    // parse the line_buff
    argc = parse_line(argv, line_buff, MAX_ARG_COUNT);

    // execute the parsed commands
    if (argc > 0)
        execute(argc, argv);
}

/**
 * @brief spwans the prompt
 *
 */
void prompt() {
    while (TRUE)
        shell();
}

void help(int argc, char** argv) {
    int i = 0;
    while (table[i].command_name != NULL) {
        printf(table[i].command_name);
        printf(" : ");
        printf(table[i].command_help);
        printf("\n");
        i++;
    }
}

// DO NOT REMOVE THESE
ADD_CMD(help, "Prints all available commands", help);
__attribute__((section (".cmd_end"))) cmd_t cmd_end_= {NULL, NULL, NULL};