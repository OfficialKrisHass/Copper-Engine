#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::RigidBody {

    float GetMass(MonoObject* rigidBody);
    void SetMass(MonoObject* rigidBody, float value);

    bool GetIsStatic(MonoObject* rigidBody);
    void SetIsStatic(MonoObject* rigidBody, bool value);
    bool GetGravity(MonoObject* rigidBody);
    void SetGravity(MonoObject* rigidBody, bool value);
    uint8 GetLockMask(MonoObject* rigidBody);
    void SetLockMask(MonoObject* rigidBody, uint8 value);

    void AddForce(MonoObject* rigidBody, const Vector3& force, uint8 forceMode);
    void AddTorque(MonoObject* rigidBody, const Vector3& torque, uint8 forceMode);

}
