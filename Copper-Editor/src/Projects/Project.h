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

		int gizmoType = 0;

		Project() : name(""), path(""), assetsPath("") {}
		Project(const std::string& name, const Filesystem::Path& path);

		void Save();
		void Load(const Filesystem::Path& path = "");

		bool BuildSolution(bool firstBuild = false) const;

		void RegenerateProjectFile() const;
		void RegenerateIDEFiles() const;
	#ifdef CU_LINUX
		void RunPremake() const;
	#endif

		operator bool() const { return name != ""; }

	};

}