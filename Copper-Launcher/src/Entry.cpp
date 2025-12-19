#include <iostream>
#include <string.h>

#include <filesystem>
#ifdef CU_WINDOWS 
#include <Windows.h>
#include <libloaderapi.h>
#endif

#ifdef CU_LINUX
#define LAUNCHER_DATA_DIRECTORY "/usr/share/copper-launcher"
#endif

std::filesystem::path dataDirectory = "";

namespace Launcher {
    
    extern int Entry(); // LauncherApp.cpp
    const std::filesystem::path& DataDirectory() { return dataDirectory; } // Base.h

}

void GetDataDirectory();

int main(int argc, char* argv[]) {

#ifdef CU_DEBUG
    for (int i = 0; i < argc; i++) {
        
        if (i == i - 2 || strcmp(argv[i], "-e") != 0) continue;

        i++;
        dataDirectory = argv[i];

    }
#endif

    if (dataDirectory.empty())
        GetDataDirectory();

    std::cout << dataDirectory << "\n";

    return Launcher::Entry();

}

void GetDataDirectory() {

    std::string tmp;

#ifdef CU_LINUX
    tmp = std::filesystem::canonical("/proc/self/exe");
#elif CU_WINDOWS
    CHAR path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    tmp = path;
#endif
    size_t pos = tmp.find_last_of(std::filesystem::path::preferred_separator);
    tmp.erase(pos, std::string::npos);

    dataDirectory = tmp;

#ifdef CU_LINUX
    if (std::filesystem::exists(dataDirectory / "assets")) return;
    dataDirectory = LAUNCHER_DATA_DIRECTORY;
#endif

}
