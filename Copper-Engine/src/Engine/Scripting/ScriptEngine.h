#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Scripting/Variable.h"

#include <CopperECS/Object.h>

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoImage MonoImage;

}

namespace Copper::ScriptEngine {

	namespace Utils {

		std::string VariableTypeToString(VariableType type);

	}

	void Initialize();
	void Shutdown();

	MonoObject* AddScriptComponent(Copper::Object& obj, std::string nameSpace, std::string scriptName);

	MonoImage* GetAssemblyImage();

	std::vector<ScriptComponent> GetScriptComponents();
	std::unordered_map<std::string, std::vector<Variable>> GetScriptFieldsAndProperties(std::string nameSpace, std::string name);

}