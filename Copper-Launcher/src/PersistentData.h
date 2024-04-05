#pragma once

#include "Base.h"
#include "ProjectEntry.h"

#include <vector>

namespace Launcher::PersistentData {

	void Load(std::vector<ProjectEntry>& entries);
	void Save(const std::vector<ProjectEntry>& entries);

	const std::string& EditorPath();

}
