#include "cupch.h"
#include "Args.h"

#ifdef CU_WINDOWS
#include <Windows.h>
#include <libloaderapi.h>
#endif

namespace Copper::Args {

    static bool initialized = false;

    static std::vector<std::string> arguments;

    static fs::path execFolder;
#ifdef CU_EDITOR
    static fs::path projectToOpenPath;
#endif

    void Initialize(uint32 argc, char* argv[]) {

        CUP_FUNCTION();

        if (initialized) {

            LogError("Can't initialize Args as it has already been initialized.");
            return;

        }
        initialized = true;
        arguments.reserve(argc);
        
        for (uint32 i = 0; i < argc; i++) {

            arguments.push_back(argv[i]);

            if (i == argc - 1) break;

#ifdef CU_DEBUG
            if (strcmp(argv[i], "-e") == 0) {

                execFolder = argv[++i];
                arguments.push_back(execFolder.string());

            }
#endif

        }

        // The last argument is an optional project to open path.
#ifdef CU_EDITOR
        if (argc > 1 && argv[argc - 2][0] != '-')
            projectToOpenPath = argv[argc - 1];
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

        CU_ASSERT(index < arguments.size(), "")
        return arguments[index];

    }

#ifdef CU_EDITOR
    const fs::path& GetProjectToOpenPath() { return projectToOpenPath; }
#endif

}

namespace Copper {

    const fs::path& ExecutableFolder() { return Args::execFolder; }

}
