OS = linux
CONFIGURATION = Debug
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)

.PHONY: clean

all: CMake/Debug/Makefile
	@${MAKE} --no-print-directory -C CMake/$(CONFIGURATION) -f Makefile
	@cp -r Copper-Editor/assets $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/lib/mono $(BUILD_DIR)/Copper-Editor/lib
	@cp Copper-Editor/imgui.ini $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/libmonosgen-2.0.a $(BUILD_DIR)/Copper-Editor

CMake/Debug/Makefile: CMakeLists.txt Copper-Engine/CMakeLists.txt Copper-Editor/CMakeLists.txt
	@cmake . -B CMake/Release -DCMAKE_BUILD_TYPE=Release
	@cmake . -B CMake/Debug -DCMAKE_BUILD_TYPE=Debug

clean:
	@rm -r CMake
	@rm -r Build