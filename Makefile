 OS = linux
CONFIGURATION = Debug
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)

.PHONY: ScriptingAPI
.PHONY: copyfiles
.PHONY: clean

all: Copper-ScriptingAPI/Makefile
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION) -f Makefile
	@cp -r Copper-Editor/assets $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/util $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/lib/mono $(BUILD_DIR)/Copper-Editor/lib
	@cp -r Copper-Editor/lib/PhysX/$(OS)/. $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/imgui.ini $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/libmonosgen-2.0.a $(BUILD_DIR)/Copper-Editor

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
	@cp -r Copper-Editor/assets $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/util $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/lib/mono $(BUILD_DIR)/Copper-Editor/lib
	@cp -r Copper-Editor/lib/PhysX/$(OS)/. $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/imgui.ini $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/libmonosgen-2.0.a $(BUILD_DIR)/Copper-Editor

clean:
	@rm -r CMake
	@rm -r Build