#pragma once

#include <Copper.h>

#include "Viewport/SceneCamera.h"

namespace Editor {

	struct Project {

		std::string name;

		std::filesystem::path path;
		std::filesystem::path assetsPath;
		std::filesystem::path lastOpenedScene;

		SceneCamera sceneCam;

		int gizmoType = 0;

		Project() : name(""), path(""), assetsPath("") {}
		Project(const std::string& name, const std::filesystem::path& path);

		void Save();
		void Load();

		void BuildSolution(bool reloadAssembly = true);

	};

}