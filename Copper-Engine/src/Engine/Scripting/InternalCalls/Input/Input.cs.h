#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoString MonoString;

}

namespace Copper::Scripting::Input {

    KeyState GetKeyState(KeyCode key);

    float GetAxis(MonoString* axis);

}
