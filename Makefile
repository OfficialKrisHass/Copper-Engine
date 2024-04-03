OS = linux
CONFIGURATION = Debug
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)

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
	
scriptapi:
	@./Copper-Editor/util/premake/premake5 --file=Copper-ScriptingAPI/premake5.lua gmake2
	@${MAKE} --no-print-directory -C Copper-ScriptingAPI -f Makefile

launcher: cmake
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION)/Copper-Launcher -f Makefile
ifeq ($(OS), linux)
	@bash scripts/linux/CopyLauncherFiles.sh $(CONFIGURATION)
endif

run:
	@./Build/linux-x86_64-$(CONFIGURATION)/Copper-Launcher/Copper-Launcher

run-editor:
	@./$(BUILD_DIR)/Copper-Editor/Copper-Editor

copy-files:
ifeq ($(OS), linux)
	@bash scripts/linux/CopyEditorFiles.sh $(CONFIGURATION)
	@bash scripts/linux/CopyLauncherFiles.sh $(CONFIGURATION)
endif
