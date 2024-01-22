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

	bool Load(const fs::path& path);
	bool Reload();

	MonoObject* AddScriptComponent(uint32 eID, const std::string& name);

	MonoObject* GetScriptEntity(uint32 eID);

	std::vector<std::string> GetScriptComponents();
	std::vector<ScriptField> GetScriptFields(const std::string& scriptName);

	MonoClass* GetVector2MonoClass();
	MonoClass* GetVector3MonoClass();

	MonoClass* GetEntityMonoClass();
	MonoClass* GetTransformMonoClass();

	MonoDomain* GetRootDomain();
	MonoDomain* GetAppDomain();

}