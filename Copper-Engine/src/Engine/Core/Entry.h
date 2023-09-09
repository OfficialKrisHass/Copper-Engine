#pragma once

#include "Engine/Core/Engine.h"
#include "Engine/Core/Args.h"

extern void AppEntryPoint(const Copper::Args& arguments);

int main(int argc, char* argv[]) {

	using namespace Copper;

	Logger::Initialize();

	Args arguments(argc, argv);
	
	AppEntryPoint(arguments);
	EngineCore::Initialize(arguments);

	return 0;

}