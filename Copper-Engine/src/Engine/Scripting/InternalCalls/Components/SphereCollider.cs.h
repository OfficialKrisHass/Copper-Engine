#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::SphereCollider {

    float GetRadius(MonoObject* collider);
    void SetRadius(MonoObject* collider, float value);

}
