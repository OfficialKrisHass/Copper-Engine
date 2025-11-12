#include "cupch.h"
#include "Args.h"

#ifdef CU_WINDOWS
#include <Windows.h>
#include <libloaderapi.h>
#endif

namespace Copper::Args {

#ifdef CU_DEBUG
    static bool initialized = false;
#endif

    static std::vector<std::string> arguments;

    static fs::path execFolder;
#ifdef CU_EDITOR
    static fs::path projectPath;
#endif

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

#ifdef CU_DEBUG
            if (strcmp(argv[i], "-e") == 0) {

                execFolder = argv[++i];
                arguments.push_back(execFolder.string());

            }
#endif

        }

        // The last argument is an optional project path.
        // Here we check if there are at least 2 arguments (the first one is the executable path)
        // and if the last argument is not the value of some option (e.g. -e).
#ifdef CU_EDITOR
        if (argc > 1 && argv[argc - 2][0] != '-')
            projectPath = argv[argc - 1];
#endif

        // We only retrieve the executable folder from the OS if it wasn't passed as an argument.
        if (!execFolder.empty()) return;

#ifdef CU_LINUX
        std::string tmp = fs::canonical("/proc/self/exe");
        size_t pos = tmp.find_last_of('/');
#elif CU_WINDOWS
        CHAR path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);

        std::string tmp = path;
        size_t pos = tmp.find_last_of('\\');
#endif
        tmp.erase(pos, std::string::npos);

        execFolder = tmp;

        LogStatus("Parsed {} command line arguments.", argc);

    }

    uint64 Count() { return arguments.size(); }
    const std::string& GetArgument(uint32 index) {

        CUP_FUNCTION();

        CU_ASSERT(index < arguments.size(), "Invalid argument index. Index: '{}'", index);
        return arguments[index];

    }

#ifdef CU_EDITOR
    const fs::path& GetProjectPath() { return projectPath; }
#endif

}

namespace Copper {

    const fs::path& ExecutableFolder() { return Args::execFolder; }

}
