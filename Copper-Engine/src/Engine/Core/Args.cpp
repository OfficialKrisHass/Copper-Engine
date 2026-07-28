#include "cupch.h"
#include "Args.h"

#ifdef CU_WINDOWS
#include <Windows.h>
#include <libloaderapi.h>
#endif

#if defined(CU_DEBUG) || defined(CU_LINUX)
#define SEPARATE_RESOURCE_DIR
#endif

namespace Copper::Args {

#ifdef CU_DEBUG
    static bool initialized = false;
#endif

    static std::vector<std::string> arguments;

    static fs::path executableDirectory;
    static fs::path dataDirectory;
#ifdef  SEPARATE_RESOURCE_DIR
    static fs::path resourceDirectory;
    void GetResourceDirectory();
#endif

    void GetExecutableDirectory();
    void GetDataDirectory();

    void Initialize(uint32 argc, char* argv[]) {

        CUP_FUNCTION();

#ifdef CU_DEBUG
        if (initialized) {

            LogError("Can't initialize Args twice.");
            return;

        }
        initialized = true;
#endif

        arguments.reserve(argc);
        for (uint32 i = 0; i < argc; i++) {

            arguments.push_back(argv[i]);

            // This may seem redundant but after this point we will be checking for options with values, which
            // are not valid if the option (starts with a -) is the last argument, e.g. there is no value argument (the next i).
            if (i == argc - 1) break;

            // In debug mode, the -e argument has priority over all. This will be used as the resource directory. 
#ifdef CU_DEBUG
            if (strcmp(argv[i], "-e") == 0) {

                resourceDirectory = argv[++i];
                arguments.push_back(resourceDirectory.string());

            }
#endif

        }

        LogStatus("Parsed {} command line arguments.", argc);

        GetExecutableDirectory();
        GetDataDirectory();

#ifdef SEPARATE_RESOURCE_DIR
        if (resourceDirectory.empty())
            GetResourceDirectory();
#endif
        
    }

    void GetExecutableDirectory() {

        CUP_FUNCTION();

#ifdef CU_LINUX
        std::string tmp = fs::canonical("/proc/self/exe");
#elif CU_WINDOWS
        CHAR path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);

        std::string tmp = path;
#endif
        size_t pos = tmp.find_last_of(fs::path::preferred_separator);
        tmp.erase(pos, std::string::npos);

        executableDirectory = tmp;

    }
    void GetDataDirectory() {

        CUP_FUNCTION();

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

    }
#ifdef SEPARATE_RESOURCE_DIR
    void GetResourceDirectory() {

        CUP_FUNCTION();

        // In portable builds, the assets will be located next to the executable

        if (fs::exists(executableDirectory / "assets"))
            resourceDirectory = executableDirectory;
#ifdef CU_LINUX // In the case of linux, resources will be found at /usr/share/
        else {

            resourceDirectory = "/usr/share";
#ifdef CU_EDITOR
            resourceDirectory /= "Copper-Editor";
#endif
        
        }
#endif

    }
#endif

    uint64 Count() { return arguments.size(); }
    const std::string& GetArgument(uint32 index) {

        CUP_FUNCTION();

        CU_ASSERT(index < arguments.size(), "Invalid argument index. Index: '{}'", index);
        return arguments[index];

    }

}

namespace Copper {

    const fs::path& ExecutableDirectory() { return Args::executableDirectory; }
    const fs::path& ResourceDirectory() {

#ifdef SEPARATE_RESOURCE_DIR
        return Args::resourceDirectory;
#else
        return Args::executableDirectory;
#endif

    }
    const fs::path& DataDirectory() { return Args::dataDirectory; }

}
