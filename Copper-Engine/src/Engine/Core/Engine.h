#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/Event.h"

#include <CopperECS/CopperECS.h>

#define GetGLFWwindow (GLFWwindow*) GetWindow().GetWindowPtr()

namespace Copper {

	void Initialize();
	void Run();
	void Shutdown();
	
	void LoadScene(Scene* scene);

	bool OnWindowResize(Event& e);
	bool OnWindowClose(Event& e);

	//Getters
	Window GetWindow();
	uint32_t GetFBOTexture();

	Scene* GetScene();

	//Setters
	void SetWindowSize(UVector2I size);

	void SetEditorRunFunc(void (*func)());
	void SetEditorUIFunc(void (*func)());

}