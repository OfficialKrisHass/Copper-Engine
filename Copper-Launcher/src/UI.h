#pragma once

#include "Base.h"

struct ImFont;

namespace Launcher::UI {

	void Initialize();

	void BeginFrame();
	void Dockspace();
	void EndFrame();

	void Shutdown();

	ImFont* AddFont(const char* path, float size);

}