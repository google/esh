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

/*
 * This is a mandatory include.
 * This has the ADD_CMD() macro
 */
#include "shell.h"

/*
 * There can be one or many function with same prototype, exposed as
 * a command on the shell. They can be in same or multiple files.
 */
int hello(int argc, char** argv) {
  for (int i = 0; i < argc; i++) {
    printf("%s ", argv[i]);
  }

  printf("\nPress ctrl + a, x to exit !\n");
  return 0;
}

/*

 * One or many such can exist per file.
 * Description: ADD_CMD(command, help string, function to be exposed)
 */
ADD_CMD(hello, "Echoes the commandline\n\tusage: hello <any string>", hello);
