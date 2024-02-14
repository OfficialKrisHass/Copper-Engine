#include "cupch.h"

namespace Copper::Args {

	std::vector<std::string> arguments;

	void Setup(uint32 argc, char* argv[]) {

		CUP_FUNCTION_START();

		arguments.resize(argc);
		for (uint32 i = 0; i < argc; i++) {

			// TODO: Find a way to make this check general, from what I could find online, there is a chance the
			// first argument may not be the executable name

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

		CUP_SCOPE_END();

	}

	uint32 Count() { return (uint32) arguments.size(); }
	const std::string& Get(uint32 index) { return arguments[index]; }

}