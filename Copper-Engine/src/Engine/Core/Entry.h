#pragma once

// This file handles the entry point of the application, it has to be included by the user, and the only thing
// that needs to be supplied is the definition for AppEntryPoint, which can be empty.

#include "Engine/Core/Engine.h"
#include "Engine/Core/Args.h"
#include "Engine/Core/UUID.h"

#include "Engine/Debug/Profiler.h"

#ifdef CU_CMAKE
#include <Config.h>
#endif

// Gets called before the engine Initialization, use this for app init
extern void AppEntryPoint();

int main(int argc, char* argv[]) {

    CUP_FUNCTION();

    using namespace Copper;

    //TODO: Start being consistent with function naming and stop eeny miny moe-ing between 'Initialize' and 'Setup' for gods sake
    Logger::Initialize();
    LogDebug("Running Copper Engine Version {}.{}.{}.{}.", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TWEAK);

    UUID::Initialize();

    Args::Initialize(argc, argv);
    Log("Data dir: '{}'", DataDirectory());
    
    AppEntryPoint();

    EngineInitialize();
    EngineRun();
    EngineShutdown();

    LogStatus("Application exiting.");

    return 0;

}
