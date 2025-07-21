#pragma once

#include "Engine/Core/Log.h"

#include "Engine/Debug/Profiler.h"

#ifdef CU_DEBUG
#define CU_CHECK(x, ...) if(x) LogError(__VA_ARGS__)
#else
#define CU_CHECK(x, ...)
#endif

#ifdef CU_DEBUG
#ifdef CU_WINDOWS
#define CU_ASSERT(x, ...) { if(!(x)) { LogError(__VA_ARGS__); ::Copper::Profiler::PrintScopeStack(); __debugbreak(); } }
#elif CU_LINUX
#define CU_ASSERT(x, ...) { if(!(x)) { LogError(__VA_ARGS__); ::Copper::Profiler::PrintScopeStack(); abort(); } }
#endif
#else
#define CU_ASSERT(x, ...)
#endif

#ifdef CU_EDITOR
#define CU_EDITOR_ASSERT(x, ...) CU_CHECK(!(x), __VA_ARGS__)
#else
#define CU_EDITOR_ASSERT(x, ...) CU_ASSERT(x, __VA_ARGS__)
#endif
