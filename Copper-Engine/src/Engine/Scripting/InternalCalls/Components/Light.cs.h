#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Light {

    Color GetColor(MonoObject* light);
    void SetColor(MonoObject* light, Color value);
    uint32 GetType(MonoObject* light);
    void SetType(MonoObject* light, uint32 value);

    float GetIntensity(MonoObject* light);
    void SetIntensity(MonoObject* light, float value);

}
