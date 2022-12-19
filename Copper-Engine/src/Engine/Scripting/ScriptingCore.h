#pragma once

#include "Engine/Core/Core.h"

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoDomain MonoDomain;

}

namespace Copper::Scripting {

	void Initialize();
	void Shutdown();

	void LoadProjectAssembly(std::filesystem::path path);
	void Reload(bool projectAlreadyLoaded = false);

	MonoObject* AddScriptComponent(int32_t obj, const std::string& name);

	std::vector<std::string> GetScriptComponents();

	MonoDomain* GetRootDomain();
	MonoDomain* GetAppDomain();

}