#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/Path.h"

#ifdef CU_WINDOWS
	#define ROOT_DIR "C:\\"
#else
	#define ROOT_DIR "/home"
#endif

namespace Copper::Utilities {

	std::string ReadFile(const Filesystem::Path& path);
	char* ReadFileBinary(const Filesystem::Path& path, uint32_t* outSize);

	Filesystem::Path SaveDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir);
	Filesystem::Path OpenDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir);

	Filesystem::Path FolderOpenDialog(const std::string& title, const Filesystem::Path& initialDir = ROOT_DIR);

}