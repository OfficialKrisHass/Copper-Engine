# EngineState
Describes the state of the engine. Certain functions and/or systems get called only during a certain state (EngineInitialize during EngineState::Entry, most Initialize functions during EngineState::Initialization, etc)

## Details
Header: Engine/Core/Engine.h
Source: No source
Namespace: Copper
Type: Enum

## Values
Entry - Default value, from the launch of any Copper-Engine application, until EngineInitialize() gets called.
Initialization - During EngineInitialize(), changed right before the end of EngineInitialize() and before the post initialization event is fired.
PostInitialization - Only during post initialization event, changed right at the beginning of EngineRun().
Running - This is the state the engine is most of the time as it stays in this frame from the beginning of the first frame, until the window is closed and the shutdown process begins. Changed in OnWindowClose() (given pre shutdown event didn't block).
Shutdown - The last state. EngineShutdown() is called during this state, all of the systems get shutdown, and the post shutdown event is fired. After that, the process terminates.

## Usage
The current engine state can be queried with GetEngineState(). Usually used to verify if the engine is in the correct state, to prevent a function being called at the wrong time, or multiple times (e.g. Renderer::Initialize being called mid frame). To do this, use the VERIFY_STATE define from Engine/Core/Engine.h, provide the expected state, and the "task" that is about to happen. On fault, the process will break and a log message will be logged into the console with the expected state, current state, and what task "caused" the assert.
