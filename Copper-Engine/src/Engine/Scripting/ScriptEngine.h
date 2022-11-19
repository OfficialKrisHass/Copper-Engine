#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Scripting/Variable.h"

#include <CopperECS/Object.h>

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;

}

namespace Copper::ScriptEngine {

	void Initialize();
	void Shutdown();

	MonoObject* AddScriptComponent(Copper::Object& obj, std::string nameSpace, std::string scriptName);

	std::vector<ScriptComponent> GetScriptComponents();
	std::unordered_map<std::string, std::vector<Variable>> GetScriptFieldsAndProperties(std::string nameSpace, std::string name);

}