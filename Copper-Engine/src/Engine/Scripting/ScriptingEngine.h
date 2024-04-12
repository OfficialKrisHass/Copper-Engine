#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Assembly.h"

namespace Copper::Scripting {

    void Initialize();
    void Shutdown();

    bool Load(const std::string& path);
    void Unload();
    bool Reload();

    const Assembly& ScriptingAPIAssembly();
    const Assembly& GameAssembly();

}
