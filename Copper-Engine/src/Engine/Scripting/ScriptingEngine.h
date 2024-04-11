#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Assembly.h"

namespace Copper::Scripting {

    void Initialize();
    void Shutdown();

    const Assembly& ScriptingAPIAssembly();

}
