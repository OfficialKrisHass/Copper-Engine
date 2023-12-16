#include "cupch.h"
#include "FileUtils.h"

#include <portable-file-dialogs.h>

namespace Copper::Utilities {

	std::string ReadFile(const Filesystem::Path& path) {

		std::string ret;
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {

			file.open(path.String());

			std::stringstream ss;

			ss << file.rdbuf();
			file.close();

			ret = ss.str();

		} catch (std::ifstream::failure e) {

			LogError("Failed to open File: {0}", path.String());

		}

		return ret;

	}
	char* ReadFileBinary(const Filesystem::Path& path, uint32_t* outSize) {
		
		std::ifstream stream;
		stream.open(path.String(), std::ios::binary | std::ios::ate);

		if (!stream) { LogError("Failed to Read Assembly File.\nPath: {0}", path.String()); return nullptr; }

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = (uint32_t) (end - stream.tellg());

		if (size == 0) { LogError("Assembly is empty.\nPath: {0}", path.String()); return nullptr; }

		char* buffer = new char[size];
		stream.read((char*) buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
		
	}

	Filesystem::Path OpenDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {

		return pfd::open_file(title, initialDir.String(), filters, false).result()[0];

	}

	Filesystem::Path SaveDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {

		return pfd::save_file(title, initialDir.String(), filters).result();

	}

	Filesystem::Path FolderOpenDialog(const std::string& title, const Filesystem::Path& initialDir) {

		return pfd::select_folder(title, initialDir.String()).result();

	}

}