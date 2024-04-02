#include "Utils.h"

namespace Launcher::Utils {

	std::string ReplaceSpaces(const std::string& string) {

		std::string ret = string;

		size_t pos = ret.find_last_of(' ');
		while (pos != std::string::npos) {

			ret.replace(pos, 1, "\" \"");
			pos = ret.find_last_of(' ', pos);

		}

		return ret;

	}

}