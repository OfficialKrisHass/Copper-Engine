#include "cupch.h"
#include "FileUtils.h"

namespace Copper::Utilities {

	std::string ReadFile(std::string path, uint32_t* size) {

		std::string ret;
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {

			file.open(path.c_str());

			std::stringstream ss;

			ss << file.rdbuf();
			file.close();

			ret = ss.str();

		} catch (std::ifstream::failure e) {

			LogError("Failed to open File: {0}", path);

		}

		return ret;

	}

}