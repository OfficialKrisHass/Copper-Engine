#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::MeshRenderer {

    MonoObject* GetMaterial(MonoObject* instance);
    void SetMaterial(MonoObject* instance, MonoObject* value);

}
