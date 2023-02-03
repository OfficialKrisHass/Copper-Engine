#pragma once

#include "Engine/Core/Engine.h"

extern void AppEntryPoint();

int main(int argc, char* argv[]) {

	using namespace Copper;

	AppEntryPoint();

	EngineCore::Initialize();
	EngineCore::Run();
	EngineCore::Shutdown();

	return 0;

}