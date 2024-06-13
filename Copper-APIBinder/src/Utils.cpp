#include "Utils.h"

#include <fstream>

namespace APIBinder {

    char* ReadFileBinary(const std::string& path, uint32* outSize) {

        std::ifstream stream;
		stream.open(path, std::ios::binary | std::ios::ate);

		if (!stream) return nullptr;

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32 size = (uint32) (end - stream.tellg());

		if (size == 0) return nullptr;

		char* buffer = new char[size];
		stream.read((char*) buffer, size);
		stream.close();

		*outSize = size;
		return buffer;

    }

}
