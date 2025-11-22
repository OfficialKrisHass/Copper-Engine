#pragma once

#include "Engine/Core/Log.h"

#include "Engine/Debug/Profiler.h"

#ifdef CU_DEBUG
#define CU_CHECK(x, ...) if(x) LogError(__VA_ARGS__)
#define CU_CHECK_RETURN(x, ret, ...) if (x) { LogError(__VA_ARGS__); return ret; }
#else
#define CU_CHECK(x, ...)
#define CU_CHECK_RETURN(x, ...)
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
#define CU_EDITOR_ASSERT_RETURN(x, ret, ...) CU_CHECK_RETURN(!(x), ret, __VA_ARGS__)
#else
#define CU_EDITOR_ASSERT(x, ...) CU_ASSERT(x, __VA_ARGS__)
#define CU_EDITOR_ASSERT_RETURN(x, ...) CU_ASSERT(x, __VA_ARGS__)
#endif
