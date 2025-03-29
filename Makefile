OS = linux
CONFIGURATION = Debug
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)

export COPPER_VERSION := $(shell cat VERSION)

cmake: CMakeLists.txt Copper-Engine/CMakeLists.txt Copper-Editor/CMakeLists.txt Copper-Launcher/CMakeLists.txt
	@cmake . -B CMake/$(CONFIGURATION) -DCMAKE_BUILD_TYPE=$(CONFIGURATION) -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	@rm -f compile_commands.json
	@ln -s CMake/$(CONFIGURATION)/compile_commands.json compile_commands.json

engine: cmake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Engine -f Makefile

editor: cmake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Editor -f Makefile
ifeq ($(OS), linux)
	@bash scripts/linux/CopyEditorFiles.sh $(CONFIGURATION)
endif
	
scriptapi: apibinder
	@./Copper-Editor/util/premake/premake5 --file=Copper-ScriptingAPI/workspace.lua gmake2
	@${MAKE} --no-print-directory -C Copper-ScriptingAPI -f Makefile

launcher: cmake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Launcher -f Makefile
ifeq ($(OS), linux)
	@bash scripts/linux/CopyLauncherFiles.sh $(CONFIGURATION)
endif

apibinder:
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-APIBinder -f Makefile

bindapi:
	@./Build/linux-x86_64-$(CONFIGURATION)/Copper-APIBinder/Copper-APIBinder $(CURDIR)/Copper-Editor

run:
ifeq ($(CONFIGURATION), Debug)
	@./$(BUILD_DIR)/Copper-Launcher/Copper-Launcher -a $(CURDIR)/Copper-Launcher/
else
	@./$(BUILD_DIR)/Copper-Launcher/Copper-Launcher
endif

run-editor:
ifeq ($(CONFIGURATION), Debug)
	@./$(BUILD_DIR)/Copper-Editor/Copper-Editor -a $(CURDIR)/Copper-Editor/
else
	@./$(BUILD_DIR)/Copper-Editor/Copper-Editor
endif

copy-files:
ifeq ($(OS), linux)
	@bash scripts/linux/CopyEditorFiles.sh $(CONFIGURATION)
	@bash scripts/linux/CopyLauncherFiles.sh $(CONFIGURATION)
endif

clean:
	@rm -rf Build
	@rm -rf CMake
	@rm -f compile_commands.json
