#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::CapsuleCollider {

    float GetRadius(MonoObject* collider);
    void SetRadius(MonoObject* collider, float value);
    float GetHeight(MonoObject* collider);
    void SetHeight(MonoObject* collider, float value);

}
