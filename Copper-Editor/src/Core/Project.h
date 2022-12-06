#pragma once

#include <Copper.h>

namespace Editor {

	struct Project {

		std::string name;

		std::filesystem::path path;
		std::filesystem::path assetsPath;
		std::filesystem::path lastOpenedScene;

		int gizmoType = 0;

		Project() : name(""), path(""), assetsPath("") {}
		Project(std::string name, std::filesystem::path path) : name(name), path(path), assetsPath(path.string() + "\\Assets") {}

		void Save();
		void Load();

		void CreateSolution();
		void BuildSolution(bool reloadAssembly = true);

	};

}