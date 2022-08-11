#pragma once

#include "Engine/Core/Engine.h"

extern void AppEntryPoint();

int main() {

	Copper::Initialize();
	AppEntryPoint();
	Copper::Run();
	Copper::Shutdown();

	return 0;

}