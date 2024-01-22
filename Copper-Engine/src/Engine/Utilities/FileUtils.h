#pragma once

#include "Engine/Core/Core.h"

#ifdef CU_WINDOWS
	#define ROOT_DIR "C:\\"
#else
	#define ROOT_DIR "/home"
#endif

#include <filesystem>

namespace Copper::Utilities {

	std::string ReadFile(const fs::path& path);
	char* ReadFileBinary(const fs::path& path, uint32* outSize);

	fs::path SaveDialog(const std::string& title, const std::vector<std::string>& filters, const fs::path& initialDir);
	fs::path OpenDialog(const std::string& title, const std::vector<std::string>& filters, const fs::path& initialDir);

	fs::path FolderOpenDialog(const std::string& title, const fs::path& initialDir = ROOT_DIR);

}