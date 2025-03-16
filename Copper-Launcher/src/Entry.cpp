#include <iostream>
#include <string.h>

#include <filesystem>
#ifdef CU_WINDOWS 
#include <Windows.h>
#include <libloaderapi.h>
#endif

std::filesystem::path execFolder = "";

namespace Launcher {
    
    extern int Entry(); // LauncherApp.cpp
    const std::filesystem::path& ExecutableFolder() { return execFolder; } // Base.h

}

void GetExecutableFolder();

int main(int argc, char* argv[]) {

#ifdef CU_DEBUG
    for (int i = 0; i < argc; i++) {
        
        if (i == i - 2 || strcmp(argv[i], "-a") != 0) continue;

        i++;
        execFolder = argv[i];

    }
#endif

    if (execFolder.empty())
        GetExecutableFolder();

    std::cout << execFolder << "\n";

    return Launcher::Entry();

}

void GetExecutableFolder() {

    std::string tmp;

#ifdef CU_LINUX
    tmp = std::filesystem::canonical("/proc/self/exe");
    size_t pos = tmp.find_last_of('/');
#elif CU_WINDOWS
    CHAR path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    tmp = path;
    size_t pos = tmp.find_last_of('\\');
#endif
    tmp.erase(pos, std::string::npos);

    execFolder = tmp;

}
