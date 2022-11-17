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

# record all the paths where .h and .hpp files are available
ALL_H_FILES		+= $(realpath $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) \
									 -type f -name "*.h") 2>/dev/null)
ALL_HPP_FILES	+= $(realpath $(shell find . $(SHELL_ROOT) $(EXTERN_SRC) \
									 -type f -name "*.hpp") 2>/dev/null)

# find the headers to be ignored
ifneq ($(IGNORE_HEADER_PATH),)
IGNORE_H	= $(shell realpath $(shell find $(IGNORE_HEADER_PATH) \
							-type f -name "*.h" ) 2>/dev/null)
IGNORE_HPP	= $(shell realpath $(shell find $(IGNORE_HEADER_PATH) \
								-type f -name "*.hpp" ) 2>/dev/null)
endif

# filter out the headers to be ignored
H_FILES		= $(filter-out $(IGNORE_H), $(ALL_H_FILES))
HPP_FILES	= $(filter-out $(IGNORE_HPP), $(ALL_HPP_FILES))

# final list of include paths!
INCLUDES = $(shell dirname $(H_FILES) $(HPP_FILES) \
			sort | uniq | \
			while read line; do echo "-I"$$line; done;)
