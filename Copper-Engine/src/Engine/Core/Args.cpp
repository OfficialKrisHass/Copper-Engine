#include "cupch.h"
#include "Args.h"

#include <filesystem>

namespace Copper::Args {

	std::vector<std::string> arguments;
  
  static std::string execFolder;

	void Setup(uint32 argc, char* argv[]) {

		CUP_FUNCTION();

    if (argc == 0) {
      
        LogError("Command line arguments count is 0 (no exec path)");
        return;
      
    }

		arguments.resize(argc);
    
    #ifdef CU_LINUX
        //std::string tmp = &argv[0][1];
        //execFolder = fs::canonical(fs::current_path().string() + tmp).string();

        execFolder = argv[0];
        execFolder.erase(execFolder.find_last_of('/'));
    #elif CU_WINDOWS
        execFolder = argv[0];

        execFolder.erase(execFolder.find_last_of('\\'));
    #endif

		for (uint32 i = 1; i < argc; i++) {

			arguments[i] = argv[i];

		}

	}

	uint32 Count() { return (uint32) arguments.size(); }
	const std::string& Get(uint32 index) { CUP_FUNCTION(); return arguments[index]; }

}

namespace Copper {

    const std::string& ExecutableFolder() { return Args::execFolder; }

}
