#include "cupch.h"
#include "Args.h"

#ifdef CU_WINDOWS
#include <Windows.h>
#include <libloaderapi.h>
#endif

namespace Copper::Args {

    static bool didRun = false;

    static std::vector<std::string> arguments;

    static fs::path execFolder;
    static fs::path projectPath;

    void Setup(uint32 argc, char* argv[]) {

        CUP_FUNCTION();

        if (didRun) {

            LogError("Args::Setup has already been run");
            return;

        }
        if (argc == 0) {

            LogError("Command line arguments count is 0 (no exec path)");
            return;

        }

        didRun = true;
        arguments.resize(argc - 1);

        for (uint32 i = 1; i < argc; i++) {

            arguments[i - 1] = argv[i];

#ifdef CU_DEBUG
            if (i == argc - 1 || arguments[i - 1] != "-a") continue;

            i++;
            execFolder = argv[i];
            arguments[i - 1] = argv[i];

#endif

        }

        if (argc != 1 && arguments[0] != "-a")
            projectPath = arguments[0];

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

    }

    uint32 Count() { return (uint32) arguments.size(); }
    const std::string& Get(uint32 index) {

        CUP_FUNCTION();

        if (index < arguments.size()) return arguments[index];

        LogError("Can't get {}-th argument as {} is the amount of arguments", index, arguments.size());

        static const std::string empty = "";
        return empty;

    }

    const fs::path& ProjectPath() { return projectPath; }

}

namespace Copper {

    const fs::path& ExecutableFolder() { return Args::execFolder; }

}
