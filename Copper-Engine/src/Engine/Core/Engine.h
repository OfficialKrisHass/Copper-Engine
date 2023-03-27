#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/Event.h"

#define GetGLFWwindow (GLFWwindow*) GetWindow().GetWindowPtr()

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

		void Initialize(int argc, char* argv[]);
		void Run();
		void Shutdown();

	}

	const Version& GetVersion();
	
	EngineState GetEngineState();
	std::string EngineStateToString(EngineState state);

	uint32_t GetNumArguments();
	const std::string& GetArgument(uint32_t index);

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

#ifdef CU_EDITOR
	void SetAcceptInputDuringRuntime(bool value);
#endif

	//Getters
	Window& GetWindow();
	UVector2I GetWindowSize();
	float GetWindowAspectRatio();

	uint32_t GetFBOTexture();

	Scene* GetScene();
	uint32_t GetNumOfEntities();

	InternalEntity* GetEntityFromID(int32_t id);
	InternalEntity* CreateEntityFromID(int32_t id, const std::string& name = "Entity", bool returnIfExists = true);

#ifdef CU_EDITOR
	bool AcceptInputDuringRuntime();
#endif

}