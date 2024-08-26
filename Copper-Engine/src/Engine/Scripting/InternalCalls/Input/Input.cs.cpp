#include "cupch.h"
#include "Input.cs.h"
#include "Engine/Input/Input.h"

#include "Engine/Scripting//InternalCalls/Utils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

namespace Copper::Scripting::Input {

    namespace Input = ::Copper::Input;

    bool IsKey(KeyCode key) {

        CUP_FUNCTION();
        return Input::IsKey(key);

    }
    bool IsKeyDown(KeyCode key) {

        CUP_FUNCTION();

        return Input::IsKeyDown(key);

    }
    bool IsKeyReleased(KeyCode key) {

        CUP_FUNCTION();
        return Input::IsKeyReleased(key);

    }

    bool IsButton(MouseCode button) {

        CUP_FUNCTION();
        return Input::IsButton(button);

    }

}
