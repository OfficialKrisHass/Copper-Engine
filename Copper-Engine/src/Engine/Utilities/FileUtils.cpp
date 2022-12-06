#include "cupch.h"
#include "FileUtils.h"

namespace Copper::Utilities {

	std::string ReadFile(std::string path) {

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
	char* ReadFileBinary(std::string path, uint32_t* outSIze) {
		
		std::ifstream stream(path, std::ios::binary | std::ios::ate);

		if (!stream) { LogError("Failed to Read Assembly File.\nPath: {0}", path); return nullptr; }

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0) { LogError("Assembly is empty.\nPath: {0}", path); return nullptr; }

		char* buffer = new char[size];
		stream.read((char*) buffer, size);
		stream.close();

		*outSIze = size;
		return buffer;
		
	}

}