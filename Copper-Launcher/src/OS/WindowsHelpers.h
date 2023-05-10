#pragma once

#ifdef CU_LINUX

#include <experimental/filesystem>
namespace std::filesystem { using path = std::experimental::filesystem::path; }

#else

#include <filesystem>

#endif

namespace Launcher::OS {

	void StartApplication(const std::filesystem::path& path, std::wstring arguments);

	std::filesystem::path OpenFolderDialog();

}