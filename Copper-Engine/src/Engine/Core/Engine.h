#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/Event.h"

#define GetGLFWwindow (GLFWwindow*) GetWindow().GetWindowPtr()

namespace Copper {

	void Initialize();
	void Run();
	void Shutdown();

	Window GetWindow();

	bool OnWindowResize(Event& e);
	bool OnWindowClose(Event& e);

	void SetEditorRunFunc(void (*func)());
	void SetEditorUIFunc(void (*func)());

}