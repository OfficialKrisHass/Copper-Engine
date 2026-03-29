#pragma once

#include "Engine/Core/Core.h"

#include "KeyCodes.h"

// Source is in Platform/<Platform>/<Platform>Input.cpp

namespace Copper { class Window; }

namespace Copper::Input {

    enum class CursorMode : uint8 {

        Normal = 0, // Cursor is visible and not restrianed.
        Hidden = 1, // Cursor is invisible but not restrained.
        Locked = 2, // Cursor is inisible and restrained.

    };

    // Input

    void Initialize(Window* window);
    void Update();

    KeyState GetKeyState(KeyCode key);

    // Getters

    CursorMode GetCursorMode();
    Vector2I GetCursorPosition();

    const Vector2I& GetCursorMotion();

    // Setters

    void SetCursorMode(CursorMode mode);
    void SetCursorPosition(int32 x, int32 y);

    inline void SetCursorPosition(const Vector2I& pos) { SetCursorPosition(pos.x, pos.y); }

}
