#pragma once

// This file needs be included by the executable, and only once

#include "Engine/Core/Engine.h"
#include "Engine/Core/Args.h"

#include "Engine/Debug/Profiler.h"

// Gets called before the engine Initialization, use this for app init
extern void AppEntryPoint();

int main(int argc, char* argv[]) {

	CUP_FUNCTION();

	using namespace Copper;

	Logger::Initialize();
	Args::Setup(argc, argv);
	
	AppEntryPoint();
	EngineCore::Initialize();

	return 0;

}