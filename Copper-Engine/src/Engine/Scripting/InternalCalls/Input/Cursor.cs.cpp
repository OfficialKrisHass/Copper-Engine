#include "cupch.h"
#include "Cursor.cs.h"
#include "Engine/Input/Input.h"

namespace Copper::Scripting::Cursor {

    bool IsVisible() {

        CUP_FUNCTION();
        return true;

    }
    void SetVisible(bool value) {

        CUP_FUNCTION();

    }

    bool IsLocked() {

        CUP_FUNCTION();
        return false;

    }
    void SetLocked(bool value) {

        CUP_FUNCTION();

    }

    Vector2 GetPosition() {

        CUP_FUNCTION();

        // TODO: Fix this
        Vector2I tmp = Input::GetCursorPosition();
        return Vector2(static_cast<float>(tmp.x), static_cast<float>(tmp.y));

    }
    void SetPosition(Vector2 value) {

        CUP_FUNCTION();
        Input::SetCursorPosition(static_cast<int32>(value.x), static_cast<int32>(value.y));

    }

}
