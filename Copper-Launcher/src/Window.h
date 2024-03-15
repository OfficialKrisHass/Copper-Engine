#pragma once

#include "Base.h"

namespace Launcher::Window {

	void Create(uint32 width, uint32 height, const char* title);
	void Update();
	void Shutdown();

	void* NativePointer();

}