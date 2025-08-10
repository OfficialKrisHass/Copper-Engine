#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Assembly.h"
#include "Engine/Scripting/Script.h"

extern "C" {

    typedef struct _MonoDomain MonoDomain;
    typedef struct _MonoClassField MonoClassField;

}

namespace Copper::Scripting {

    void Initialize();
    void Shutdown();

    bool Load(const fs::path& assemblyPath);
    void Unload(bool clearManagedRefMap = true);
    bool Reload();

    MonoDomain* AppDomain();

    const Assembly& ScriptingAPIAssembly();
    const Assembly& GameAssembly();

    typedef std::unordered_map<std::string, Script> ScriptMap;
    const ScriptMap& ComponentScripts();
    const Script* GetScript(const std::string& name);

    MonoClassField* UnmanagedPtrField(); 

}
