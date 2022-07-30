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

#include "mem.h"
#include "shell.h"

static int on = 0;

void setup() {
	SYS_CTRL->RCGC2 |= clk_port_F;
	GPIO_PORT_F->GPIODIR = pin_1;
	GPIO_PORT_F->GPIODEN = pin_1;
}

void led(int turn_on){
  if(turn_on)
    GPIO_PORT_F->DATA[pin_1] = pin_1;
  else
    GPIO_PORT_F->DATA[pin_1] = ~pin_1;
}

int blink(int argc, char** argv) {
    if (on) {
        led(0);
        on = 0;
    } else {
        led(1);
        on = 1;
    }

    return 0;
}

ADD_CMD(blink, "Toggles LED", blink);
