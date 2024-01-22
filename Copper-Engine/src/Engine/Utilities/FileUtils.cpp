#include "cupch.h"
#include "FileUtils.h"

#include <portable-file-dialogs.h>

namespace Copper::Utilities {

	std::string ReadFile(const fs::path& path) {

		std::string ret;
		std::ifstream file;

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {

			file.open(path.string());

			std::stringstream ss;

			ss << file.rdbuf();
			file.close();

			ret = ss.str();

		} catch (std::ifstream::failure e) {

			LogError("Failed to open File: {0}", path.string());

		}

		return ret;

	}
	char* ReadFileBinary(const fs::path& path, uint32* outSize) {
		
		std::ifstream stream;
		stream.open(path.string(), std::ios::binary | std::ios::ate);

		if (!stream) { LogError("Failed to Read Assembly File.\nPath: {0}", path.string()); return nullptr; }

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32 size = (uint32) (end - stream.tellg());

		if (size == 0) { LogError("Assembly is empty.\nPath: {0}", path.string()); return nullptr; }

		char* buffer = new char[size];
		stream.read((char*) buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
		
	}

	fs::path OpenDialog(const std::string& title, const std::vector<std::string>& filters, const fs::path& initialDir) {

		std::vector<std::string> ret = pfd::open_file(title, initialDir.string(), filters).result();
		return ret.size() != 0 ? ret[0] : "";

	}

	fs::path SaveDialog(const std::string& title, const std::vector<std::string>& filters, const fs::path& initialDir) {

		return pfd::save_file(title, initialDir.string(), filters).result();

	}

	fs::path FolderOpenDialog(const std::string& title, const fs::path& initialDir) {

		return pfd::select_folder(title, initialDir.string()).result();

	}

}