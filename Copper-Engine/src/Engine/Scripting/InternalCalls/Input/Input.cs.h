#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Scripting::Input {

    bool IsKey(KeyCode key);
    bool IsKeyDown(KeyCode key);
    bool IsKeyReleased(KeyCode key);

    bool IsButton(MouseCode button);

}
