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

#include "a2i.h"

int atoi(const char* str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        res = res * 10 + str[i] - '0';
    }
    return res;
}

uint32_t atoh(const char* str) {
    uint32_t hex = 0;
    uint32_t value = 0;

    for (uint32_t i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            value = str[i] - '0';
        } else if (str[i] >= 'a' && str[i] <= 'f') {
            value = str[i] - 'a' + 10;
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            value = str[i] - 'A' + 10;
        } else {
            continue;
        }

        // make space for the new nibble on the right
        hex = hex << 4;
        hex |= value;
    }

    return hex;
}
