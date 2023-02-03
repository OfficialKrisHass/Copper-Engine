#pragma once

#include "Engine/Core/Core.h"

namespace Copper::UI {

	void Initialize();
	void Shutdown();

	void Begin();
	void End();

	void LoadFont(const std::string& path, float fontSize = 18.0f);

}