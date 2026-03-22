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
std::filesystem::path resourceDirectory = "";

namespace Launcher { extern int Entry(); }

void GetResourceDirectory();
void GetDataDirectory();

int main(int argc, char* argv[]) {

#ifdef CU_DEBUG
    for (int i = 0; i < argc; i++) {
        
        if (i == i - 2 || strcmp(argv[i], "-e") != 0) continue;
        resourceDirectory = argv[++i];

    }
#endif

    if (resourceDirectory.empty())
        GetResourceDirectory();

    GetDataDirectory();

    return Launcher::Entry();

}

void GetResourceDirectory() {

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

    resourceDirectory = tmp;

#ifdef CU_LINUX
    if (std::filesystem::exists(dataDirectory / "assets")) return;
    resourceDirectory = LAUNCHER_DATA_DIRECTORY;
#endif

}
void GetDataDirectory() {

#ifdef CU_LINUX
    // We need to respect XDG_DATA_HOME, as some users may change the environemt variable to

    const char* xdgData = std::getenv("XDG_DATA_HOME");
    if (xdgData == nullptr || *xdgData == '\0') {

        dataDirectory = std::getenv("HOME");
        dataDirectory /= ".local/share";

    } else
        dataDirectory = xdgData;
#elif CU_WINDOWS
    dataDirectory = std::getenv("appdata");
#endif
    dataDirectory /= "Copper-Launcher";

}

namespace Launcher {

    const std::filesystem::path& DataDirectory() { return dataDirectory; }
    const std::filesystem::path& ResourceDirectory() { return resourceDirectory; }

}
