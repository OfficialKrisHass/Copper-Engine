#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/Path.h"

namespace Copper::Utilities {

	std::string ReadFile(const Filesystem::Path& path);
	char* ReadFileBinary(const Filesystem::Path& path, uint32_t* outSize);

	Filesystem::Path SaveDialog(const std::string& filter, const Filesystem::Path& initialDir);
	Filesystem::Path OpenDialog(const std::string& filter, const Filesystem::Path& initialDir);

	Filesystem::Path FolderOpenDialog();

}