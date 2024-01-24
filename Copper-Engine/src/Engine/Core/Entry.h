#pragma once

// This file needs be included by the executable, and only once

#include "Engine/Core/Engine.h"
#include "Engine/Core/Args.h"

// Gets called before the engine Initialization, use this for app init
extern void AppEntryPoint();

int main(int argc, char* argv[]) {

	using namespace Copper;

	Logger::Initialize();
	Args::Setup(argc, argv);
	
	AppEntryPoint();
	EngineCore::Initialize();

	return 0;

}