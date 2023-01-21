#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Field.h"

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {

	void Initialize();
	void Shutdown();

	void LoadProjectAssembly(std::filesystem::path path);
	void Reload(std::filesystem::path path = "", bool initScriptComponents = true);

	MonoObject* AddScriptComponent(int32_t obj, const std::string& name);

	std::vector<std::string> GetScriptComponents();
	std::vector<ScriptField> GetScriptFields(std::string scriptName);

	MonoClass* GetCopperObjectClass();

	MonoDomain* GetRootDomain();
	MonoDomain* GetAppDomain();

}