#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Transform {

    Vector3 GetPosition(MonoObject* transform);
    void SetPosition(MonoObject* transform, Vector3 value);
    void GetRotation(MonoObject* transform, Quaternion* ret);
    void SetRotation(MonoObject* transform, const Quaternion* value);
    Vector3 GetScale(MonoObject* transform);
    void SetScale(MonoObject* transform, Vector3 value);

    Vector3 GetGlobalPosition(MonoObject* transform);
    void GetGlobalRotation(MonoObject* transform, Quaternion* ret);
    Vector3 GetGlobalScale(MonoObject* transform);

    Vector3 GetForward(MonoObject* transform);
    Vector3 GetRight(MonoObject* transform);
    Vector3 GetUp(MonoObject* transform);

}
