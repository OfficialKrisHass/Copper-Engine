#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Scripting {

	void Initialize();
	void Shutdown();

	void LoadProjectAssembly(std::filesystem::path path);
	void Reload(bool projectAlreadyLoaded = false);

}