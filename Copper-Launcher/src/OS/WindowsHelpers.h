#pragma once

#include <filesystem>

namespace Launcher::OS {

	void StartApplication(const std::filesystem::path& path, std::wstring arguments);

	std::filesystem::path OpenFolderDialog();

}