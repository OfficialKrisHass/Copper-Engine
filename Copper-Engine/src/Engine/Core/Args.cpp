#include "cupch.h"
#include "Args.h"

#ifdef CU_WINDOWS
#include <Windows.h>
#include <libloaderapi.h>
#endif

#if defined(CU_LINUX) && defined(CU_EDITOR)
#define EDITOR_DATA_DIRECTORY "/usr/share/copper-editor"
#endif

namespace Copper::Args {

#ifdef CU_DEBUG
    static bool initialized = false;
#endif

    static std::vector<std::string> arguments;

    static fs::path dataDirectory;

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

            // 1. In debug mode, the -e argument has priority over all. This will be used as the data folder
#ifdef CU_DEBUG
            if (strcmp(argv[i], "-e") == 0) {

                dataDirectory = argv[++i];
                arguments.push_back(dataDirectory.string());

            }
#endif

        }

        // 2. On non debug builds or when the -e argument was not passed, we use the executable path for windows,
        //    on linux we use the executable path for non editor builds, and for editor builds we check if the
        //    executable path contains the assets directory (portable builds), if not we use EDITOR_DATA_DIRECTORY
        //
        // TODO: Currently this means that other copper-engine applications can not be placed in the /usr/bin directory
        //       because in that case the data directory would be /usr/bin/ which is an invalid place to put assets.

        if (!dataDirectory.empty()) return;

#ifdef CU_LINUX
        std::string tmp = fs::canonical("/proc/self/exe");
#elif CU_WINDOWS
        CHAR path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);

        std::string tmp = path;
#endif
        size_t pos = tmp.find_last_of(fs::path::preferred_separator);
        tmp.erase(pos, std::string::npos);

        dataDirectory = tmp;

#ifdef EDITOR_DATA_DIRECTORY
        if (fs::exists(dataDirectory / "assets")) return;
        dataDirectory = EDITOR_DATA_DIRECTORY; 
#endif

        LogStatus("Parsed {} command line arguments.", argc);

    }

    uint64 Count() { return arguments.size(); }
    const std::string& GetArgument(uint32 index) {

        CUP_FUNCTION();

        CU_ASSERT(index < arguments.size(), "Invalid argument index. Index: '{}'", index);
        return arguments[index];

    }

}

namespace Copper {

    const fs::path& DataDirectory() { return Args::dataDirectory; }

}
