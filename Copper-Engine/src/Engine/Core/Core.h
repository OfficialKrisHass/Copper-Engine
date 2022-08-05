#pragma once

#ifdef CU_WINDOWS

	#ifdef CU_DLL

		#define COPPER_API __declspec(dllexport)

	#else

		#define COPPER_API __declspec(dllimport)

	#endif

#else

	#error This version of the Copper Engine only supports Windows!

#endif

#include "Engine/Core/Log.h"