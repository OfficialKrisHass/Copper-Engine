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

	void LoadProjectAssembly(const std::filesystem::path& path);
	void Reload(const std::filesystem::path& path = "", bool initScriptComponents = true);

	MonoObject* AddScriptComponent(uint32_t obj, const std::string& name);

	std::vector<std::string> GetScriptComponents();
	std::vector<ScriptField> GetScriptFields(const std::string& scriptName);

	MonoClass* GetVector2MonoClass();
	MonoClass* GetVector3MonoClass();

	MonoClass* GetCopperObjectMonoClass();

	MonoDomain* GetRootDomain();
	MonoDomain* GetAppDomain();

}