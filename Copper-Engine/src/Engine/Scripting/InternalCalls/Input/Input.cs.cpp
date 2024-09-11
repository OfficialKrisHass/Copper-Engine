#include "cupch.h"
#include "Input.cs.h"
#include "Engine/Input/Input.h"

#include "Engine/Scripting//InternalCalls/Utils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

#ifdef CU_EDITOR
#define CHECK_ACCEPTING_INPUT(retValue) if (!IsGameAcceptingInput()) return retValue;

extern bool IsGameAcceptingInput();
#else
#define CHECKCHECK_ACCEPTING_INPUT()
#endif

namespace Copper::Scripting::Input {

    namespace Input = ::Copper::Input;

    bool IsKey(KeyCode key) {

        CUP_FUNCTION();

        CHECK_ACCEPTING_INPUT(false);
        return Input::IsKey(key);

    }
    bool IsKeyDown(KeyCode key) {

        CUP_FUNCTION();

        CHECK_ACCEPTING_INPUT(false);
        return Input::IsKeyDown(key);

    }
    bool IsKeyReleased(KeyCode key) {

        CUP_FUNCTION();

        CHECK_ACCEPTING_INPUT(false);
        return Input::IsKeyReleased(key);

    }

    bool IsButton(MouseCode button) {

        CUP_FUNCTION();

        CHECK_ACCEPTING_INPUT(false);
        return Input::IsButton(button);

    }

}
