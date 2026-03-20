OS = linux
CONFIGURATION = Debug
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)
INSTALL_PREFIX = /usr

PROJECT_FILES = VERSION Makefile CMakeLists.txt Copper-Engine/CMakeLists.txt Copper-Editor/CMakeLists.txt Copper-Launcher/CMakeLists.txt Copper-APIBinder/CMakeLists.txt Copper-ScriptingAPI/premake5.lua Copper-ScriptingAPI/workspace.lua

ENGINE = $(BUILD_DIR)/Copper-Engine/libCopper-Engine.a
EDITOR = $(BUILD_DIR)/Copper-Editor/Copper-Editor
LAUNCHER = $(BUILD_DIR)/Copper-Launcher/Copper-Launcher
SCRIPTING_API = $(BUILD_DIR)/Copper-ScriptingAPI/Copper-ScriptingAPI.dll
API_BINDER = $(BUILD_DIR)/Copper-APIBinder/Copper-APIBinder

ifeq ($(CONFIGURATION), Debug)
	EDITOR_ARGS = -e $(CURDIR)/Copper-Editor
	LAUNCHER_ARGS = -e $(CURDIR)/Copper-Launcher

	PHYSX_CONFIG = debug
	SCRIPTING_API_CONFIG = debug
else
	EDITOR_ARGS =
	LAUNCHER_ARGS =

	PHYSX_CONFIG = release
	SCRIPTING_API_CONFIG = release
endif

export COPPER_VERSION := $(shell cat VERSION)
export CMAKE_POLICY_VERSION_MINIMUM=3.5

.PHONY: projects libraries engine editor launcher scriptapi apibinder bindapi
.PHONY: run run-editor

.PHONY: copyfiles clean

all: $(LAUNCHER)

## Higher level targets

projects: CMake

# Build targets

libraries: Copper-Engine/lib/.stamp

engine: Copper-Engine/lib/.stamp
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Engine -f Makefile

editor: Copper-Engine/lib/.stamp CMake 
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Editor -f Makefile
	@python scripts/post_build.py editor $(CONFIGURATION) $(OS)
launcher: CMake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Launcher -f Makefile
	@python scripts/post_build.py launcher $(CONFIGURATION) $(OS)

scriptapi: CMake
	@${MAKE} --no-print-directory -C Copper-ScriptingAPI -f Makefile config=$(SCRIPTING_API_CONFIG)
apibinder: CMake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-APIBinder -f Makefile
bindapi: CMake
	@cp $(SCRIPTING_API) $(CURDIR)/Copper-Editor/assets
	@$(API_BINDER) $(CURDIR)/Copper-Editor
	@touch Copper-APIBinder/.stamp

install:
	@cmake --install CMake/$(CONFIGURATION)

# Run targets

run:
	@./$(LAUNCHER) $(LAUNCHER_ARGS)

run-editor:
	@./$(EDITOR) $(EDITOR_ARGS)

# Util

copyfiles:
	@python scripts/post_build.py libraries $(CONFIGURATION) $(OS)
	@python scripts/post_build.py editor $(CONFIGURATION) $(OS)
	@python scripts/post_build.py launcher $(CONFIGURATION) $(OS)

clean:
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION) -f Makefile clean
	@${MAKE} --no-print-directory -C Copper-Engine/lib/PhysX/physx/compiler/linux-gcc-$(PHYSX_CONFIG) -f Makefile clean
	@rm -rf Build
	@rm -rf CMake
	@rm -f Copper-Engine/lib/.stamp
	@rm -f Copper-APIBinder/.stamp
	@rm -f Copper-Editor/assets/Copper-ScriptingAPI.dll
	@rm -f compile_commands.json

## Lower level target

# Setup

CMake: $(PROJECT_FILES)
	@cmake . -B CMake/$(CONFIGURATION) -DCMAKE_BUILD_TYPE=$(CONFIGURATION) -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	@rm -f compile_commands.json
	@ln -s CMake/$(CONFIGURATION)/compile_commands.json compile_commands.json
	@echo ""
	@./Copper-Editor/assets/util/premake/premake5 --file=Copper-ScriptingAPI/workspace.lua gmake2

# Build targets

Copper-Engine/lib/.stamp:
	@${MAKE} --no-print-directory -C Copper-Engine/lib/PhysX/physx/compiler/linux-gcc-cpu-only-$(PHYSX_CONFIG) -f Makefile
	@${MAKE} --no-print-directory -C Copper-Engine/lib/mono -f Makefile
	@${MAKE} --no-print-directory -C Copper-Engine/lib/mono -f Makefile install
	@python scripts/post_build.py libraries $(CONFIGURATION) $(OS)
	@touch Copper-Engine/lib/.stamp
