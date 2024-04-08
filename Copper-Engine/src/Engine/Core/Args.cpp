#include "cupch.h"
#include "Args.h"

#ifdef CU_WINDOWS
#include <Windows.h>
#include <libloaderapi.h>
#endif

namespace Copper::Args {

    static bool didRun = false;

	static std::vector<std::string> arguments;
  static std::string execFolder;

	void Setup(uint32 argc, char* argv[]) {

        if (didRun) {

            LogError("Args::Setup has already been run");
            return;

        }
    if (argc == 0) {
      
        LogError("Command line arguments count is 0 (no exec path)");
        return;
      
    }
		CUP_FUNCTION();
        didRun = true;

		arguments.resize(argc - 1);
    
    #ifdef CU_LINUX
        execFolder = fs::canonical("/proc/self/exe");
        size_t pos = execFolder.find_last_of('/');
    #elif CU_WINDOWS
        CHAR path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);

        execFolder = path;
        size_t pos = execFolder.find_last_of('\\');
    #endif
        execFolder.erase(pos, std::string::npos);

		for (uint32 i = 1; i < argc; i++)
			arguments[i - 1] = argv[i];

	}

	uint32 Count() { return (uint32) arguments.size(); }
	const std::string& Get(uint32 index) { CUP_FUNCTION(); return arguments[index]; }

}

namespace Copper {

    const std::string& ExecutableFolder() { return Args::execFolder; }

}
