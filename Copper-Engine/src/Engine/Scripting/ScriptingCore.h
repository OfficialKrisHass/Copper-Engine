#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/Path.h"

#include "Engine/Scripting/Field.h"

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {

	void Initialize();
	void Shutdown();

	bool LoadProjectAssembly(const Filesystem::Path& path);
	bool Reload(const Filesystem::Path& path = "", bool initScriptComponents = true);

	MonoObject* AddScriptComponent(uint32_t eID, const std::string& name);

	std::vector<std::string> GetScriptComponents();
	std::vector<ScriptField> GetScriptFields(const std::string& scriptName);

	MonoClass* GetVector2MonoClass();
	MonoClass* GetVector3MonoClass();

	MonoClass* GetEntityMonoClass();

	MonoDomain* GetRootDomain();
	MonoDomain* GetAppDomain();

}