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

    bool Load(const std::string& path);
    void Unload();
    bool Reload();

    MonoDomain* AppDomain();

    const Assembly& ScriptingAPIAssembly();
    const Assembly& GameAssembly();

    MonoClassField* UnmanagedPtrField(); 

    const std::vector<Script>& ScriptComponents();

}
