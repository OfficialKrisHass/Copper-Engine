#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Collider {

    uint8 GetType(MonoObject* collider);
    MonoObject* GetRigidBody(MonoObject* collider);

    bool GetTrigger(MonoObject* collider);
    void SetTrigger(MonoObject* collider, bool value);
    Vector3 GetCenter(MonoObject* collider);
    void SetCenter(MonoObject* collider, Vector3 value);

}
