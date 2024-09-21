#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Scripting::Cursor {

    bool IsVisible();
    void SetVisible(bool value);

    bool IsLocked();
    void SetLocked(bool value);

    Vector2 GetPosition();
    void SetPosition(Vector2 value);

}
