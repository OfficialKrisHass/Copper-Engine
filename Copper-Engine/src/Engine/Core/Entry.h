#pragma once

// This file needs be included by the executable, and only once

#include "Engine/Core/Engine.h"
#include "Engine/Core/Args.h"

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
    Log("Running Copper Engine Version {}.{}.{}.{}.", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TWEAK);

    Args::Initialize(argc, argv);
    
    AppEntryPoint();

    EngineInitialize();
    EngineRun();
    EngineShutdown();

    Log("Application exiting.");

    return 0;

}
