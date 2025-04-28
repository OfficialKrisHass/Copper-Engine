#pragma once

#include "Engine/Core/Core.h"

#include "KeyCodes.h"

// Source is in Platform/<Platform>/<Platform>Input.cpp

namespace Copper { class Window; }

namespace Copper::Input {

    // Input

    void Initialize(Window* window);
    void Update();

    KeyState GetKeyState(KeyCode key);

    // Getters

    bool GetCursorVisible();
    bool GetCursorLocked();
    Vector2I GetCursorPosition();

    const Vector2I& GetCursorPosChange();

    // Setters

    void SetCursorVisible(bool visible);
    void SetCursorLocked(bool locked);
    void SetCursorPosition(int32 x, int32 y);

    inline void SetCursorPosition(const Vector2I& pos) { SetCursorPosition(pos.x, pos.y); }

}
