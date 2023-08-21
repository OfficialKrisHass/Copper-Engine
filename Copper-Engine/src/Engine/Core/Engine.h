#pragma once

#include "Engine/Core/Core.h"

#define GetGLFWwindow (GLFWwindow*) GetWindow().GetWindowPtr()

#define VERIFY_STATE(state, task) CHECK((EngineCore::GetEngineState() == state), "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineCore::EngineStateToString(state), EngineCore::EngineStateToString(EngineCore::GetEngineState()))

namespace Copper {

	class Event;

	namespace EngineCore {

		enum class EngineState : uint8_t {

			Entry,
			Initialization,
			PostInitialization,
			Running,
			Shutdown,

		};

		void Initialize(int argc, char* argv[]);

		EngineState GetEngineState();
		std::string EngineStateToString(EngineState state);

		uint32_t GetNumArguments();
		const std::string& GetArgument(uint32_t index);

	}

	// Engine Events

	void AddPostInitEventFunc(std::function<void()> func);

	void AddUpdateEventFunc(std::function<void()> func);
	void AddUIUpdateEventFunc(std::function<void()> func);

	void AddPreShutdownEventFunc(std::function<bool(const Event&)> func);
	void AddPostShutdownEventFunc(std::function<void()> func);

	// Editor Misc.

#ifdef CU_EDITOR
	void SetAcceptInputDuringRuntime(bool value);
	bool AcceptInputDuringRuntime();
#endif

}