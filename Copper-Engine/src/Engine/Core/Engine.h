#pragma once

#include "Engine/Core/Core.h"

#define GetGLFWwindow (GLFWwindow*) GetWindow().GetWindowPtr()

#define VERIFY_STATE(state, task) CHECK((EngineCore::GetEngineState() == state), "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineCore::EngineStateToString(state), EngineCore::EngineStateToString(EngineCore::GetEngineState()))

namespace Copper {

	class Event;
	class Args;

	namespace EngineCore {

		enum class EngineState : uint8_t {

			Entry,
			Initialization,
			PostInitialization,
			Running,
			Shutdown,

		};

		void Initialize(const Args& arguments);

		EngineState GetEngineState();
		std::string EngineStateToString(EngineState state);

		const Args* GetArguments();

	}

	// Engine Events

	void AddPostInitEventFunc(std::function<void()> func);

	void AddUpdateEventFunc(std::function<void()> func);
	void AddUIUpdateEventFunc(std::function<void()> func);

	void AddPreShutdownEventFunc(std::function<bool(const Event&)> func);
	void AddPostShutdownEventFunc(std::function<void()> func);

	// Game

	float DeltaTime();

	// Editor Misc.

#ifdef CU_EDITOR
	void SetAcceptInputDuringRuntime(bool value);
	bool AcceptInputDuringRuntime();
#endif

}