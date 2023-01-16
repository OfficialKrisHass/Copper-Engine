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

	bool OnWindowResize(const Event& e);
	bool OnWindowClose(const Event& e);
	bool OnKeyPressed(const Event& e);

	//Getters
	Window GetWindow();
	UVector2I GetWindowSize();

	uint32_t GetFBOTexture();

	Scene* GetScene();
	Object GetObjectFromID(int32_t id);

	//Setters
	void SetWindowSize(UVector2I size);

	void SetRenderScene(bool value);

	void SetEditorRunFunc(void (*func)());
	void SetEditorUIFunc(void (*func)());

	void SetEditorOnKeyPressedFunc(bool (*func)(const Event&));
	void SetEditorOnWindowCloseFunc(bool (*func)(const Event&));

}