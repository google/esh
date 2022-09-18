#
# Copyright 2022 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

SILENT_BUILD       ?= 0

# This is a Mandatory Variable! Raise an error if this is not set
TOOLCHAIN_PREFIX   ?=
ifeq ($(TOOLCHAIN_PREFIX),)
    $(info TOOLCHAIN_PREFIX not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: TOOLCHAIN_PREFIX = arm-none-eabi- )
    $(error )
endif

# This is a Mandatory Variable! Raise an error if this is not set
STARTUP            ?=
ifeq ($(STARTUP),)
    $(info STARTUP not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: STARTUP = cortex-m.S )
    $(error )
endif

USER_LAYOUT_FILE   ?=

# If the USER_LAYOUT_FILE is not specified then we expect the following
# to be set by the user!
RAM_BASE_PHYSICAL  ?=
RAM_SIZE           ?=
ifeq ($(USER_LAYOUT_FILE),)
ifeq ($(RAM_BASE_PHYSICAL),)
    $(info RAM_BASE_PHYSICAL not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: RAM_BASE_PHYSICAL = 0x00000000 )
    $(error )
endif # ($(RAM_BASE_PHYSICAL),)

ifeq ($(RAM_SIZE),)
    $(info RAM_SIZE not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: RAM_SIZE = 0x4000 )
    $(error )
endif # ($(RAM_SIZE),)
endif # ifeq ($(USER_LAYOUT_FILE),)

ROM_BASE_PHYSICAL  ?=
ROM_SIZE           ?=
ifneq ($(ROM_BASE_PHYSICAL),)
ifeq ($(ROM_SIZE),)
    $(info ROM_SIZE not set!)
    $(info This is expected to be set in project level Makefile by user)
    $(info Example: ROM_SIZE = 0x4000 )
    $(error )
endif
endif

# Use these defaults if none provided by user
PROJECT            ?= shell
PROJECT_ROOT       ?= $(CURDIR)
DOCS               ?= $(SHELL_ROOT)/../docs/index.md
ECHO_INIT_VALUE    ?= 0x1
EXTERN_SRC         ?=
IGNORE_SRC_PATH    ?=
DEFINES            ?=

# build with complete set of feature by default
SHELL_LITE ?= 0

# ------------------------------ optimization ----------------------------------
OPTIMIZATION ?= 0
