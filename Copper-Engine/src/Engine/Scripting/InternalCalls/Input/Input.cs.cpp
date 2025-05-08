#include "cupch.h"
#include "Input.cs.h"
#include "Engine/Input/Input.h"
#include "Engine/Input/AxisManager.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

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

    KeyState GetKeyState(KeyCode key) {

        CUP_FUNCTION();

        CHECK_ACCEPTING_INPUT(KeyState::None);
        return Input::GetKeyState(key);

    }

    float GetAxis(MonoString* axis) {

        CUP_FUNCTION();

        CHECK_ACCEPTING_INPUT(0);

        std::string axisName;
        MonoUtils::MonoStringToString(axis, axisName);

        return Input::GetAxis(axisName);

    }
    float GetMouseAxis(MonoString* axis) {

        CUP_FUNCTION();

        CHECK_ACCEPTING_INPUT(0);

        std::string axisName;
        MonoUtils::MonoStringToString(axis, axisName);

        return Input::GetMouseAxis(axisName);

    }

}
