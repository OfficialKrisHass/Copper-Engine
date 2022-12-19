#pragma once

#include "Engine/Core/Core.h"

#include <cupch.h>

extern "C" {

	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoString MonoString;
	typedef struct _MonoDomain MonoDomain;

}

namespace Copper::Scripting::MonoUtils {

	MonoAssembly* LoadAssembly(std::filesystem::path path);

	std::string MonoToString(MonoString* string);
	MonoString* StringToMono(std::string string);

	std::string RemoveNamespace(std::string& name);

}