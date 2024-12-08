#include <iostream>
#include <string>

#ifdef CU_LINUX
#include <filesystem>
#elif CU_WINDOWS
#include <Windows.h>
#include <libloaderapi.h>
#endif

std::string execFolder = "";

namespace Launcher {
    
    extern int Entry(); // LauncherApp.cpp
    const std::string& ExecutableFolder() { return execFolder; } // Base.h

}

int main(int argc, char* argv[]) {

#ifdef CU_LINUX
    execFolder = std::filesystem::canonical("/proc/self/exe");
    size_t pos = execFolder.find_last_of('/');
#elif CU_WINDOWS
    CHAR path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    execFolder = path;
    size_t pos = execFolder.find_last_of('\\');
#endif
    execFolder.erase(pos, std::string::npos);
    std::cout << execFolder << "\n";

    return Launcher::Entry();

}
