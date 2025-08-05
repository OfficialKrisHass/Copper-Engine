# Summary
Describes the state of the engine. Certain functions and/or systems get called only during a certain state (EngineInitialize during EngineState::Entry, most Initialize functions during EngineState::Initialization, etc)

# Header
Engine/Core/Engine.h
# Source
No source
# Namespace
Copper
# Type
Enum

# Values
Entry#Default value at startup, Logger, args and EngineInitialize are called during this state.
Initialization#Set at the beggining of EngineInitialize, every engine system is initialized during this state.
PostInitialization#Set at the end of EngineInitialize, only the post initialization event is fired during this state.
Running#Set at the beginning of EngineRun, the engine loop keeps running while in this state.
Shutdown#Set at the end of OnWindowClose, the engine and all of its systems are shut down in this state, and the application exits.
