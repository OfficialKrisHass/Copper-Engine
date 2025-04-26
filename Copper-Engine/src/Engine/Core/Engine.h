#pragma once

#include "Engine/Core/Core.h"

// Check if the engine is in the correct state
#define VERIFY_STATE(state, task) CU_ASSERT((GetEngineState() == state), "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineStateToString(state), EngineStateToString(GetEngineState()))

namespace Copper {

    class Event;
    class SimpleEvent;

    enum class EngineState : uint8 {

        Entry, // Before Engine::Initialize is called
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
    const char* EngineStateToString(EngineState state);

    // Engine Events

    SimpleEvent& GetPostInitEvent();

    SimpleEvent& GetUpdateEvent();
    SimpleEvent& GetUIUpdateEvent();

    Event& GetPreShutdownEvent();
    SimpleEvent& GetPostShutdownEvent();

    // Game

    float GetDeltaTime();

}
