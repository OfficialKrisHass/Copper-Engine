#pragma once

#include "Engine/Core/Core.h"

// Check if the engine is in the correct state
#define VERIFY_STATE(state, task) CU_ASSERT((EngineCore::GetEngineState() == state), "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineCore::EngineStateToString(state), EngineCore::EngineStateToString(EngineCore::GetEngineState()))

namespace Copper {

	class Event;

	namespace EngineCore {

		enum class EngineState : uint8 {

			Entry, // Before Engine::Initialize is called
			Initialization, // Only for internal use, logger, scripting, etc get initialized during this state
			PostInitialization, // Before The first frame is rendered, AppPostInitEven is called during this
			Running, // Logical duh
			Shutdown, // The last frame

		};

		// Internal function aka DO NOT USE THIS
		void Initialize();

		EngineState GetEngineState();
		std::string EngineStateToString(EngineState state);

	}

	// Engine Events

	void AddPostInitEventFunc(std::function<void()> func); // Called after all the systems are initialized and just before the first frame is started

	void AddUpdateEventFunc(std::function<void()> func); // Fired every frame
	void AddUIUpdateEventFunc(std::function<void()> func); // Fired every frame, in the UI rendering time window

	void AddPreShutdownEventFunc(std::function<bool(const Event&)> func); // Called when the engine is closed, return false to stop the engine from shutting down
	void AddPostShutdownEventFunc(std::function<void()> func); // Called after all the systems are shut down

	// Game

	float DeltaTime();

	// Editor Misc.

#ifdef CU_EDITOR
	void SetAcceptInputDuringRuntime(bool value);
	bool AcceptInputDuringRuntime();
#endif

}