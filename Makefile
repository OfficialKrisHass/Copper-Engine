OS = linux
CONFIGURATION = Release
BUILD_DIR = Build/$(OS)-x86_64-$(CONFIGURATION)

all: CMake
	@${MAKE} --no-print-directory -C CMake -f Makefile
	@cp -r Copper-Editor/assets $(BUILD_DIR)/Copper-Editor
	@cp -r Copper-Editor/lib/mono $(BUILD_DIR)/Copper-Editor/lib
	@cp Copper-Editor/imgui.ini $(BUILD_DIR)/Copper-Editor
	@cp Copper-Editor/libmonosgen-2.0.a $(BUILD_DIR)/Copper-Editor

CMake: CMakeLists.txt Copper-Engine/CMakeLists.txt Copper-Editor/CMakeLists.txt
	@cmake . -B CMake -DCMAKE_BUILD_TYPE=Release

clean:
	@rm -r CMake
	@rm -r Build