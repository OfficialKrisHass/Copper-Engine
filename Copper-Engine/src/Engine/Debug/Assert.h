#pragma once

#include "Engine/Core/Log.h"

#include "Engine/Debug/Profiler.h"

#define CHECK(x, ...) { if(!(x)) { LogError(__VA_ARGS__); } }

#ifdef CU_DEBUG
#define CU_ASSERT(x, ...) { if(!(x)) { LogError(__VA_ARGS__); ::Copper::Profiler::CrashHandler(-1); __debugbreak(); } }
#else
#define CU_ASSERT(x, ...) (x)
#endif