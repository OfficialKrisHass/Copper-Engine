#pragma once

#include <string>

namespace Launcher::UI {

	void Initialize();
	void Shutdown();

	void Begin();
	void End();

	void LoadFont(const std::string& path, float fontSize = 18.0f);

}