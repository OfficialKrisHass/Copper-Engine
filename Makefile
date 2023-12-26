OS = linux
CONFIGURATION = Debug
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)

.PHONY: ScriptingAPI
.PHONY: copyfiles
.PHONY: clean

all: Copper-ScriptingAPI/Makefile copyfiles
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION) -f Makefile

CMake: CMakeLists.txt Copper-Engine/CMakeLists.txt Copper-Editor/CMakeLists.txt
	@echo -------- Running CMake: Release --------
	@cmake . -B CMake/Release -DCMAKE_BUILD_TYPE=Release
	@echo -------- Running CMake: Debug --------
	@cmake . -B CMake/Debug -DCMAKE_BUILD_TYPE=Debug

Copper-ScriptingAPI/Makefile: Copper-ScriptingAPI/premake5.lua
	@echo -------- Building Files for Copper-ScriptingAPI --------
	@./premake/premake5 --file=Copper-ScriptingAPI/premake5.lua gmake2
	@${MAKE} --no-print-directory -C Copper-ScriptingAPI -f Makefile

ScriptingAPI:
	@${MAKE} --no-print-directory -C Copper-ScriptingAPI -f Makefile

copyfiles:
	@mkdir -p $(BUILD_DIR)/Copper-Editor/lib/mono
	@cp -r Copper-Editor/assets $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/util $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/lib/mono/lib $(BUILD_DIR)/Copper-Editor/lib/mono
	@cp -r Copper-Editor/lib/PhysX/$(OS)/$(CONFIGURATION)/. $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/lib/mono/bin/linux/$(CONFIGURATION)/libmonosgen-2.0.a $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/imgui.ini $(BUILD_DIR)/Copper-Editor

clean:
	@rm -r CMake
	@rm -r Build