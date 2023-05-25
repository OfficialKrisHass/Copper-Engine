#pragma once

#include <Copper.h>

#include "Projects/Project.h"

namespace Editor {

	void CreateTemplateFromProject(const Project& project);
	void CreateProjectFromTemplate(const Copper::Filesystem::Path& templatePath, Project& project);

}