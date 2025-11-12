#pragma once

#include "Engine/Core/Core.h"

// Check if the engine is in the correct state
#define VERIFY_STATE(state, task) CU_ASSERT((GetEngineState() == state), "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineStateToString(state), EngineStateToString(GetEngineState()))

namespace Copper {

    class Event;
    class SimpleEvent;

    enum class EngineState : uint8 {

        Entry, // Before EngineInitialize is called
        Initialization, // Only for internal use, logger, scripting, etc get initialized during this state
        PostInitialization, // Before The first frame is rendered, AppPostInitEven is called during this
        Running, // Logical duh
        Shutdown, // The last frame

    };

    // Internal functions aka DO NOT USE THIS
    void EngineInitialize();
    void EngineRun();
    void EngineShutdown();

    EngineState GetEngineState();
    inline constexpr const char* EngineStateToString(EngineState state) {

        switch (state) {

            case EngineState::Entry: return "Entry"; break;
            case EngineState::Initialization: return "Initialization"; break;
            case EngineState::PostInitialization: return "Post Initialization"; break;
            case EngineState::Running: return "Running"; break;
            case EngineState::Shutdown: return "Shutdown"; break;

        }

        LogError("Invalid engine state: {}", static_cast<uint8>(state));
        return "";

    }

    // Engine Events

    SimpleEvent& GetPostInitEvent(); // Called after Engine initialization is finished, EngineState::PostInitialization.

    SimpleEvent& GetUpdateEvent(); // Called every frame after the scene was updated and renderered, EngineState::Running.
    SimpleEvent& GetUIUpdateEvent(); // called every frame while the main UI context is active, EngineState::Running.

    Event& GetPreShutdownEvent(); // Called when a shutdown was requested, if this event blocks, the shutdown will be prevented. EngineState::Running.
    SimpleEvent& GetPostShutdownEvent(); // Called after the engine was shut down, EngineState::Shutdown.

    // Game

    float GetDeltaTime();

}
