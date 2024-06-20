#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Camera {

    float GetFov(MonoObject* camera);
    void SetFov(MonoObject* camera, float value);

}
