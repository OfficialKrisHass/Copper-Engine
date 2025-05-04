# Summary
Core header that holds the EngineInitialize/Run/Shutdown functions, EngineState, and other core engine functions and features.

# Header
Engine/Core/Engine.h
# Source
Engine/Core/Engine.cpp
# Namespace
Copper
# Type
Header

# Functions
void EngineInitialize()#Internal, Initializes the engine and all of its systems, post initialization event is fired at the end of this function. DO NOT USE. 
void EngineRun()#Internal, Starts the main engine loop. DO NOT USE.
void EngineShutdown()#Internal, Shutdowns the engine and all of its systems, post shutdown is fired at the end of this function. No other functions are called after this. DO NOT USE.

EngineState GetEngineState()#Returns the current engine state.
const char* EngineStateToString(EngineState state)#Converts the engine state into a string.

SimpleEvent& GetPostInitEvent()#Returns a reference to the post initialization event.

SimpleEvent& GetUpdateEvent()#Returns a reference to the update event.
SimpleEvent& GetUIUpdateEvent()#Returns a reference to the ui update event.

Event& GetPreShutdownEvent()#Returns a reference to the pre shutdown event.
SimpleEvent& GetPostShutdownEvent()#Returns a reference to the post shutdown event.

float GetDeltaTime()#Returns the time between the start of the last frame, and start of the current frame (aka how long it took to finish the last frame, delta time).

# Defines
VERIFY_STATE(state, task)#Asserts that the current engine state is equal to state. Task is a string.
