#include "cupch.h"
#include "Cursor.cs.h"
#include "Engine/Input/Input.h"

namespace Copper::Scripting::Cursor {

    bool IsVisible() {

        CUP_FUNCTION();
        return Input::IsCursorVisible();

    }
    void SetVisible(bool value) {

        CUP_FUNCTION();
        Input::SetCursorVisible(value);

    }

    bool IsLocked() {

        CUP_FUNCTION();
        return Input::IsCursorLocked();

    }
    void SetLocked(bool value) {

        CUP_FUNCTION();
        return Input::SetCursorLocked(value);

    }

    Vector2 GetPosition() {

        CUP_FUNCTION();

        double x, y;
        Input::GetCursorPosition(&x, &y);

        return Vector2((float) x, (float) y);

    }
    void SetPosition(Vector2 value) {

        CUP_FUNCTION();
        Input::SetCursorPosition(value.x, value.y);

    }

}
