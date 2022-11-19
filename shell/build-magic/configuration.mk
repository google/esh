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

# ------------------------------ configuration ---------------------------------
# Only printed when issued - make

define build_config
	@echo Detected Configuration
	@echo "*"
	@echo "├── PROJECT             : $(PROJECT)"
	@echo "├── BUILD USER          : $(BUILD_USER)"
	@echo "├── BUILD HOST          : $(BUILD_HOST)"
	@echo "├── SHELL VERSION       : $(SHELL_VERSION)"
	@echo "├── USER REPO VERSION   : $(USER_REPO_VERSION)"
	@echo "├── SHELL ROOT          : $(shell realpath $(SHELL_ROOT))"
	@echo "├── PROJECT ROOT        : $(shell realpath $(PROJECT_ROOT))"
	@echo "├── OPTIMIZATION        : $(OPTIMIZATION)"
	@echo "*"
	@echo "├── TOOLCHAIN           : $(TOOLCHAIN_PREFIX)"
	@echo "├── STARTUP             : $(STARTUP_FILE)"
	@echo "├── LAYOUT FILE         : $(LD_FILE)"
	@echo "├── RAM BASE            : $(RAM_BASE_PHYSICAL)"
	@echo "├── RAM SIZE            : $(RAM_SIZE)"
	@echo "├── ROM BASE            : $(ROM_BASE_PHYSICAL)"
	@echo "└── ROM SIZE            : $(ROM_SIZE)"
	@echo
	@echo ">> If incorrect, please set these Variables in Makefile"
	@echo ">> Refer the documentation here: $(shell realpath $(DOCS))"
	@echo
endef

.NOTPARALLEL: config
### config: Prints the build configuration
config:
ifeq ($(SILENT_BUILD),1)
	@
else
	@$(call build_config)
endif

.PHONY: help
help:
	@echo ""
	@grep -hE '^###.*$$' $(MAKEFILE_LIST) | \
	sed 's/###//' | sed 's/:/|/' | \
	column -t  -s '|'
