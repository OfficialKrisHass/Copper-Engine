#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::BoxCollider {

    Vector3 GetSize(MonoObject* collider);
    void SetSize(MonoObject* collider, Vector3 value);

}
