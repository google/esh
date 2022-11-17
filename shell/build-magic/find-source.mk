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

# ------------------------------ source ----------------------------------------
# discover the C source files in project and shell directory
ALL_SRC_C	+= $(shell realpath $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) \
				-type f -name "*.c" ) 2>/dev/null)
ALL_SRC_CPP	+= $(shell realpath $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) \
				-type f -name "*.cpp" ) 2>/dev/null)
ifneq ($(IGNORE_SRC_PATH),)
IGNORE_C	= $(shell realpath $(shell find $(IGNORE_SRC_PATH) \
				-type f -name "*.c" ) 2>/dev/null)
IGNORE_CPP	= $(shell realpath $(shell find $(IGNORE_SRC_PATH) \
				-type f -name "*.cpp" ) 2>/dev/null)
endif

SRC_C 	= $(filter-out $(IGNORE_C), $(ALL_SRC_C))
SRC_CPP = $(filter-out $(IGNORE_CPP), $(ALL_SRC_CPP))

# discover the ASM source files in project directory
SRC_ASM  += $(shell realpath $(shell find . -type f -name "*.S") \
			2>/dev/null)

# search the startup file in bot project and shell directory
STARTUP_FILE ?= $(shell realpath $(shell find . $(SHELL_ROOT) \
				-type f -name "$(STARTUP)" -o  -name "$(STARTUP).S") \
				2>/dev/null)

# startup file may be included twice, filter it from the source
FILTERED_SRC_ASM = $(filter-out $(STARTUP_FILE), $(SRC_ASM))
