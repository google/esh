**Important disclaimer**:

    This is not an officially supported Google product

# Early Bringup

Implementation of a `bare-metal shell`. Can be used for early bringup of embedded platforms or for the purposes of learning and research.

## Install Dependencies

In order to be able to run the `qemu` based examples in the `example/` directory, you would need to have the following installed:
```
sudo apt install -y binutils \
                    make \
                    gcc-aarch64-linux-gnu \
                    qemu-system-aarch64 \
                    gdb-multiarch \
                    gcc-riscv64-linux-gnu \
                    qemu-system-riscv64
```
This would allow compiling, running and debugging the code.

## Compiling target

Get into any of the directories within `exaple/`, for the purposes of demo we will use the `riscv64` example.
```
cd example/riscv64
```
To build the source, simply execute
```
make
```
This will result in output something like below:
```
Detected Configuration
*
|- PROJECT    : shell
|- STARTUP    : riscv64.S
|- TOOLCHAIN  : riscv64-linux-gnu-
*
|- RAM BASE   : 0x80000000
|- RAM SIZE   : 0x4000000
|- UART BASE  : 0x10000000
`- STACK SIZE : 0x1000

>> If incorrect, please set these Variables in Makefile <<

Building...
...
<source file list>
...
section size:
   text    data     bss     dec     hex filename
   1564     266       0    1830     726 shell.elf
generating shell.bin
Done!
```
`shell.elf` and `shell.bin` are the compiled outputs. These can be run on the riscv64 qemu target.

## Running on Qemu
Once done with the compilation above, use the following command to run the binaries on the qemu target.
```
make run
```
You should see a prompt as below:
```
❯ make run
Build: Thu Sep 30 09:57:45 2021
#
```
Typing `help` should reveal all the available commands. Like below:
```
❯ make run
Build: Thu Sep 30 09:57:45 2021
# help
help : Prints all available commands
hello : Echoes the commandline
        usage: hello <any string>
#
```
`hello` command is implemented in the `hello.c` file and serves as an example of how more commands can be added.

**Exit QEMU**: `ctrl+a` then `x` to exit.

# Debugging
If you want to debug by using `gdb` you would need to open two terminals wihtin the location `example/riscv64/` and start the `qemu` run with the board halted on terminal and then connect `gdb` to it from the second terminal.

On first terminal execute:
```
make run_debug
```
This will start the qemu target and hold the CPU in reset.

On the second terminal, execute:
```
make debug
```

This will then start `gdb` and attach it to the qemu target. You should see output as below:
```
❯ make debug
...
Remote debugging using localhost:1234
0x0000000000001000 in ?? ()
(gdb)
```

You can now use familiar gdb commands at the `(gdb)` prompt, like `break`, `continue`, `run` etc to debug the target further.

### Using GDB dashboard
If you want to have a graphics gdb dashboard. You should consider using one by [cyrus-and](https://github.com/cyrus-and/gdb-dashboard). To install it simply execute the following:
```
wget -P ~ https://git.io/.gdbinit
```
Be sure, you also have the `pygments` installed. If you don't already have it, use the following to install it:
```
pip install pygments
```

After this is done. Next time you execute the `make debug` command, you should see a nice console that would show you the processor internals, like below:
```
❯ make debug
...
Reading symbols from shell.elf...
Remote debugging using localhost:1234
0x0000000000001000 in ?? ()
─── Assembly ───────────────────────────────────────────────────────────────────
 0x0000000000001000  ? auipc    t0,0x0
 0x0000000000001004  ? addi    a2,t0,40
 0x0000000000001008  ? csrr    a0,mhartid
 0x000000000000100c  ? ld    a1,32(t0)
 0x0000000000001010  ? ld    t0,24(t0)
 0x0000000000001014  ? jr    t0
 0x0000000000001018  ? unimp
 0x000000000000101a  ? 0x8000
 0x000000000000101c  ? unimp
 0x000000000000101e  ? unimp
─── Breakpoints ────────────────────────────────────────────────────────────────
─── Expressions ────────────────────────────────────────────────────────────────
─── History ────────────────────────────────────────────────────────────────────
─── Memory ─────────────────────────────────────────────────────────────────────
─── Registers ──────────────────────────────────────────────────────────────────
    zero 0x0000000000000000     ra 0x0000000000000000     sp 0x0000000000000000
      gp 0x0000000000000000     tp 0x0000000000000000     t0 0x0000000000000000
      t1 0x0000000000000000     t2 0x0000000000000000     fp 0x0000000000000000
      s1 0x0000000000000000     a0 0x0000000000000000     a1 0x0000000000000000
      a2 0x0000000000000000     a3 0x0000000000000000     a4 0x0000000000000000
      a5 0x0000000000000000     a6 0x0000000000000000     a7 0x0000000000000000
      s2 0x0000000000000000     s3 0x0000000000000000     s4 0x0000000000000000
      s5 0x0000000000000000     s6 0x0000000000000000     s7 0x0000000000000000
      s8 0x0000000000000000     s9 0x0000000000000000    s10 0x0000000000000000
     s11 0x0000000000000000     t3 0x0000000000000000     t4 0x0000000000000000
      t5 0x0000000000000000     t6 0x0000000000000000     pc 0x0000000000001000
─── Source ─────────────────────────────────────────────────────────────────────
─── Stack ──────────────────────────────────────────────────────────────────────
[0] from 0x0000000000001000
─── Threads ────────────────────────────────────────────────────────────────────
[1] id 1 from 0x0000000000001000
─── Variables ──────────────────────────────────────────────────────────────────
────────────────────────────────────────────────────────────────────────────────
>>>
```
gdb commands can be used as usual at the `>>>` prompt.

## Cleaning
Execute the following to clean the current working directory
```
make clean
```

## Source Code Headers

Every file containing source code must include copyright and license
information. This is to help well-intentioned people avoid accidental copying that
doesn't comply with the license.

Apache header:

    Copyright 2021 Google LLC

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
