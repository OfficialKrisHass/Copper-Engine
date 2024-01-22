#pragma once

#include <Copper.h>

#include "Viewport/SceneCamera.h"

namespace Editor {

	using namespace Copper;

	struct Project {

	public:
		std::string name;

		fs::path path;
		fs::path assetsPath;
		fs::path lastOpenedScene;

		Copper::uint32 gizmoType = 0;

		Project() : name(""), path(""), assetsPath("") {}
		Project(const std::string& name, const fs::path& path);

		void Save() const;
		void Load(const fs::path& path = "");

		bool BuildSolution(bool firstBuild = false) const;

		void RegenerateProjectFile() const;
		void RegenerateIDEFiles() const;
	#ifdef CU_LINUX
		void RunPremake() const;
	#endif

		operator bool() const { return name != ""; }

	};

}