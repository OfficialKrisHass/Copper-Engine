#include "cupch.h"

namespace Copper::Args {

	std::vector<std::string> arguments;

	void Setup(uint32 argc, char* argv[]) {

		arguments.resize(argc);
		for (uint32 i = 0; i < argc; i++) {

			if (i == 0) {

				std::string tmp = argv[0];
			#ifdef CU_WINDOWS
				if (tmp.find("Copper-Editor.exe") != std::string::npos)
			#elif CU_LINUX
				if (tmp.find("Copper-Editor") != std::string::npos)
			#endif
					continue;
			}

			arguments[i] = argv[i];

		}

	}

	uint32 Count() { return (uint32) arguments.size(); }
	const std::string& Get(uint32 index) { return arguments[index]; }

}