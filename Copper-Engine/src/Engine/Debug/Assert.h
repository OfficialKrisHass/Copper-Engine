#pragma once

#include "Engine/Core/Log.h"

#define CHECK(x, ...) { if(!(x)) { LogError(__VA_ARGS__); } }

#ifdef CU_DEBUG
#define CU_ASSERT(x, ...) { if(!(x)) { LogError(__VA_ARGS__); __debugbreak(); } }
#else
#define CU_ASSERT(x, ...)
#endif