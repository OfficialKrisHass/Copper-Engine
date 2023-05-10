#pragma once

#ifdef CU_LINUX

#include <experimental/filesystem>
namespace std::filesystem { using path = std::experimental::filesystem::path; }

#else

#include <filesystem>

#endif

#include <string>

namespace Launcher {

	struct Project {

	public:
		std::string name;
		std::filesystem::path path;

		Project() = default;
		Project(const std::string& name, const std::filesystem::path& path) : name(name), path(path) {}

	};

}