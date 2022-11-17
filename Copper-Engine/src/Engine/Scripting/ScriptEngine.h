#pragma once

#include "Engine/Core/Core.h"

#include <CopperECS/Object.h>

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;

}

namespace Copper::ScriptEngine {

	void Initialize();
	void Shutdown();

	MonoObject* AddScriptComponent(Copper::Object& obj, std::string nameSpace, std::string scriptName);

	std::vector<std::string> GetScriptComponentNames();

}