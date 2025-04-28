# Engine.h
Core header that holds the EngineInitialize/Run/Shutdown functions, EngineState, and other core engine functions and features.

## Details
Header: Engine/Core/Engine.h
Source: Engine/Core/Engine.cpp
Namespace: Copper
Type: Header

Included only in Copper.h so no need to include manually in Copper-Engine applications.

## Enums
EngineState : uint8 - Describes the state of the engine.

## Functions
void EngineInitialize() - Internal, Initializes the engine and all of it's systems, calls post initialization event. Asserts if state is EngineState::Entry. DO NOT USE.
void EngineRun() - Internal, Starts the main game loop and updates the window and all other systems, renders the scene, UI, etc, every single frame, keeps running as long as engine state is EngineState::Running. Asserts if state is EngineState::PostInitialization. DO NOT USE.
void EngineShutdown() - Internal, Shutdowns the engine andd all of its systems, calls post shutdown event at the end. Asserts if state is EngineState::Shutdown. DO NOT USE.

EngineState GetEngineState() - Returns the current EngineState.
const char* EngineStateToString(EngineState state) - Converts state to a string representation.

SimpleEvent& GetPostInitEvent() - Returns the post initialization event. This event is called during EngineState::PostInitialization, after the engine is fully initialized, and just before the first frame gets rendered.

SimpleEvent& GetUpdateEvent() - Returns the update event. This event is called during EngineState::Running and is called every frame, after the window has been updated and scene has been rendered, but before the Renderer has finished the frame and before the UI gets rendered.
SimpleEvent& GetUIUpdateEvent() - Returns the UI update event. This event is called during EngineState::Running and is called every frame after the update event and renderer have finished the frame, and before the Input is updated and frame fully finished rendering. The main UI context frame has been setup so ImGui is available.

Event& GetPreShutdownEvent() - Returns the pre shutdown event. This event is called during EngineState::Running, but after the Window Close has been requested, and before EngineState::Shutdown. If this event blocks, the window will not be shut down, and engine will keep on running.
SimpleEvent& GetPostShutdownEvent() - Returns the post shutdown event. This event is called during EngineState::Shutdown and after all the systems have been shutdown. This is the very last thing that gets called before the process stops.

float GetDeltaTime() - Returns delta time, the elapsed time (in seconds) since the last frame has begun rendering (aka how long it took to render the previous frame)

## Defines
VERIFY_STATE(state, task) = CU_ASSERT((GetEngineState() == state), "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineStateToString(state), EngineStateToString(GetEngineState())) - Asserts whether the EngineState is equal to state. task should be a string of the attempted process/task, that could not have been done because of the invalid state, used for logging purposes.
