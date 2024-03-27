#pragma once

#include "Engine/Core/Log.h"

#include "Engine/Debug/Profiler.h"

#define CHECK(x, ...) { if(!(x)) { LogError(__VA_ARGS__); } }

#ifdef CU_DEBUG
#ifdef CU_WINDOWS
#define CU_ASSERT(x, ...) { if(!(x)) { LogError(__VA_ARGS__); ::Copper::Profiler::CrashHandler(-1); __debugbreak(); } }
#elif CU_LINUX
#define CU_ASSERT(x, ...) { if(!(x)) { LogError(__VA_ARGS__); ::Copper::Profiler::CrashHandler(-1); exit(-1); } }
#endif
#else
#define CU_ASSERT(x, ...)
#endif