#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoString MonoString;

}

namespace Copper::Scripting::Input {

    bool IsKey(KeyCode key);
    bool IsKeyDown(KeyCode key);
    bool IsKeyReleased(KeyCode key);

    bool IsButton(MouseCode button);

    float GetAxis(MonoString* axis);

}
