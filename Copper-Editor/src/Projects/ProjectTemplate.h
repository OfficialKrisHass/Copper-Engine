#pragma once

#include <Copper.h>

#include "Projects/Project.h"

namespace Editor {

	void CreateTemplateFromProject(const Project& project);
	void CreateProjectFromTemplate(const std::filesystem::path& templatePath, Project& project);

}