#pragma once

#include "Engine/Core/Core.h"

#include "cupch.h"

namespace Copper::Utilities {

	std::string ReadFile(std::string path);
	char* ReadFileBinary(std::string path, uint32_t* size);

	std::string SaveDialog(const char* filter, std::filesystem::path initialDir);
	std::string OpenDialog(const char* filter, std::filesystem::path initialDir);

	std::string FolderOpenDialog();

}