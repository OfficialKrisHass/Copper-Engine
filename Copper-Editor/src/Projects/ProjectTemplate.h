#pragma once

#include <Copper.h>

#include "Projects/Project.h"

namespace Editor {

	void CreateTemplateFromProject(const Project& project);
	void CreateProjectFromTemplate(const Copper::fs::path& templatePath, Project& project);

}
