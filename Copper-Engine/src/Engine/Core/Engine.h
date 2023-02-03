#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/Event.h"

#define GetGLFWwindow (GLFWwindow*) GetWindow().GetWindowPtr()

#define VERSION_STAGE "Beta"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_DEV 1

namespace Copper {

	class Scene;
	class InternalEntity;

	enum class EngineState : uint8_t {

			Entry,
			Initialization,
			PostInitialization,
			Running,
			Shutdown,

	};

	namespace EngineCore {

		void Initialize();
		void Run();
		void Shutdown();

	}
	
	EngineState GetEngineState();
	std::string EngineStateToString(EngineState state);

	//Engine Events
	void AddPostInitEventFunc(std::function<void()> func);

	void AddUpdateEventFunc(std::function<void()> func);
	void AddUIUpdateEventFunc(std::function<void()> func);

	void AddPreShutdownEventFunc(std::function<bool(const Event&)> func);
	void AddPostShutdownEventFunc(std::function<void()> func);

	bool IsRuntimeRunning();

	//Setters
	void SetWindowSize(const UVector2I& size);

	void SetRenderScene(bool value);

	//Getters
	Window& GetWindow();
	UVector2I GetWindowSize();
	float GetWindowAspectRatio();

	uint32_t GetFBOTexture();

	Scene* GetScene();
	uint32_t GetNumOfEntities();

	InternalEntity* GetEntityFromID(int32_t id);
	InternalEntity* CreateEntityFromID(int32_t id, const std::string& name = "Entity", bool returnIfExists = true);

}