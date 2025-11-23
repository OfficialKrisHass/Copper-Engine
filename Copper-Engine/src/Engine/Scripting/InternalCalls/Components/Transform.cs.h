#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Transform {

    // Position rotation and scale

    Vector3 GetPosition(MonoObject* transform);
    void GetRotation(MonoObject* transform, Quaternion* ret);
    Vector3 GetScale(MonoObject* transform);

    void SetPosition(MonoObject* transform, Vector3 value);
    void SetRotation(MonoObject* transform, const Quaternion* value);
    void SetScale(MonoObject* transform, Vector3 value);

    // Globals

    Vector3 GetGlobalPosition(MonoObject* transform);
    void GetGlobalRotation(MonoObject* transform, Quaternion* ret);
    Vector3 GetGlobalScale(MonoObject* transform);

    void SetGlobalPosition(MonoObject* transform, Vector3 value);
    void SetGlobalRotation(MonoObject* transform, Quaternion* value);
    void SetGlobalScale(MonoObject* transform, Vector3 value);

    // Directionals

    Vector3 GetForward(MonoObject* transform);
    Vector3 GetRight(MonoObject* transform);
    Vector3 GetUp(MonoObject* transform);

    // Parent & Child

    MonoObject* GetParent(MonoObject* transform);
    void SetParent(MonoObject* transform, MonoObject* parent);

    MonoObject* GetChild(MonoObject* transform, uint32 index);
    uint32 GetChildCount(MonoObject* transform);

    void AddChild(MonoObject* transform, MonoObject* child);
    void RemoveChild(MonoObject* transform, uint32 index);
    void RemoveChildTransform(MonoObject* transform, MonoObject* child);

}
