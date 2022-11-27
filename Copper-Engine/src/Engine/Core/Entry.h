#pragma once

#include "Engine/Core/Engine.h"

extern void AppEntryPoint();
extern void AppShutdown();

int main() {

	Copper::Initialize();
	AppEntryPoint();
	Copper::Run();
	Copper::Shutdown();
	AppShutdown();

	return 0;

}