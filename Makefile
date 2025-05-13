OS = linux
CONFIGURATION = Debug
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)

PROJECT_FILES = Makefile CMakeLists.txt Copper-Engine/CMakeLists.txt Copper-Editor/CMakeLists.txt Copper-Launcher/CMakeLists.txt Copper-APIBinder/CMakeLists.txt Copper-ScriptingAPI/premake5.lua Copper-ScriptingAPI/workspace.lua

ENGINE = $(BUILD_DIR)/Copper-Engine/libCopper-Engine.a
EDITOR = $(BUILD_DIR)/Copper-Editor/Copper-Editor
LAUNCHER = $(BUILD_DIR)/Copper-Launcher/Copper-Launcher
SCRIPTING_API = $(BUILD_DIR)/Copper-ScriptingAPI/Copper-ScriptingAPI.dll
API_BINDER = $(BUILD_DIR)/Copper-APIBinder/Copper-APIBinder

ifeq ($(CONFIGURATION), Debug)
	EDITOR_ARGS = -e $(CURDIR)/Copper-Editor
	LAUNCHER_ARGS = -e $(CURDIR)/Copper-Launcher
else
	EDITOR_ARGS =
	LAUNCHER_ARGS =
endif

export COPPER_VERSION := $(shell cat VERSION)

.PHONY: projects engine editor launcher scriptapi apibinder bindapi
.PHONY: run run-editor

.PHONY: copyfiles clean

all: $(LAUNCHER)

## Higher level targets

projects: CMake

# Build targets
#
engine: Copper-APIBinder/.stamp
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Engine -f Makefile

editor:
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Editor -f Makefile
	@python scripts/post_build.py editor $(CONFIGURATION) $(OS)
launcher: CMake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Launcher -f Makefile
	@python scripts/post_build.py launcher $(CONFIGURATION) $(OS)

scriptapi: CMake
	@${MAKE} --no-print-directory -C Copper-ScriptingAPI -f Makefile
apibinder: CMake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-APIBinder -f Makefile
bindapi: Copper-APIBinder/.stamp

# Run targets

run: $(LAUNCHER) $(EDITOR)
	@./$(BUILD_DIR)/Copper-Launcher/Copper-Launcher $(LAUNCHER_ARGS)

run-editor: $(EDITOR)
	@./$(BUILD_DIR)/Copper-Editor/Copper-Editor $(EDITOR_ARGS)

# Util

copyfiles:
	@python scripts/post_build.py editor $(CONFIGURATION) $(OS)
	@python scripts/post_build.py launcher $(CONFIGURATION) $(OS)

clean:
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION) -f Makefile clean
	@rm -rf Build
	@rm -rf CMake
	@rm -f Copper-APIBinder/.stamp
	@rm -f Copper-Editor/assets/Copper-ScriptingAPI.dll
	@rm -f compile_commands.json

## Lower level target

# Setup

CMake: $(PROJECT_FILES)
	@cmake . -B CMake/$(CONFIGURATION) -DCMAKE_BUILD_TYPE=$(CONFIGURATION) -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	@rm -f compile_commands.json
	@ln -s CMake/$(CONFIGURATION)/compile_commands.json compile_commands.json
	@echo ""
	@./Copper-Editor/util/premake/premake5 --file=Copper-ScriptingAPI/workspace.lua gmake2

# Build targets

Copper-APIBinder/.stamp:
	@cp $(SCRIPTING_API) $(CURDIR)/Copper-Editor/assets
	@$(API_BINDER) $(CURDIR)/Copper-Editor
	@touch Copper-APIBinder/.stamp
