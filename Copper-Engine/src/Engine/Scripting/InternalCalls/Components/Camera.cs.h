#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Camera {

    float GetFov(MonoObject* camera);
    void SetFov(MonoObject* camera, float value);

    float GetNearPlane(MonoObject* camera);
    void SetNearPlane(MonoObject* camera, float value);
    float GetFarPlane(MonoObject* camera);
    void SetFarPlane(MonoObject* camera, float value);

}
