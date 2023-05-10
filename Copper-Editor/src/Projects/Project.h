#pragma once

#include <Copper.h>

#include "Viewport/SceneCamera.h"

namespace Editor {

	using namespace Copper;

	struct Project {

		std::string name;

		Filesystem::Path path;
		Filesystem::Path assetsPath;
		Filesystem::Path lastOpenedScene;

		SceneCamera sceneCam;

		int gizmoType = 0;

		Project() : name(""), path(""), assetsPath("") {}
		Project(const std::string& name, const Filesystem::Path& path);

		void Save();
		void Load(const Filesystem::Path& path = "");

		void BuildSolution(bool firstBuild = false);

	};

}