#pragma once

#include <filesystem>

namespace Launcher {

	struct Project {

	public:
		std::string name;
		std::filesystem::path path;

		Project() = default;
		Project(const std::string& name, const std::filesystem::path& path) : name(name), path(path) {}

	};

}