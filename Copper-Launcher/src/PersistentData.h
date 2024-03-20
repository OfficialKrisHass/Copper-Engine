#pragma once

#include "Base.h"

namespace Launcher::PersistentData {

	void Load();
	void Save();

	const std::string& EditorPath();

}