#include "cupch.h"
#include "AxisManager.h"

#include "Engine/Core/Engine.h"

#define MOUSE_AXIS_ERR "Axis '{}' is a mouse axis, use GetMouseAxis()."
#define NORMAL_AXIS_ERR "Axis '{}' is a normal axis, use GetAxis()."
#define INVALID_AXIS_ERR "Axis '{}' could not be found, double check the name."

namespace Copper::Input {

    std::unordered_map<std::string, Axis> axises;
    std::unordered_map<std::string, MouseAxis> mouseAxises;

    float GetMouseAxis(const std::string& name);

    void InitializeAxisManager() {

        CUP_FUNCTION();
        VERIFY_STATE(EngineState::Initialization, "Initialize the Axis Manager");

        AddAxis("Keys_WS", KeyCode::W, KeyCode::S);
        AddAxis("Keys_DA", KeyCode::D, KeyCode::A);

        AddMouseAxis("Mouse X", true);
        AddMouseAxis("Mouse Y", false);

    }

    float GetAxis(const std::string& name) {

        CUP_FUNCTION();

#ifdef CU_EDITOR
#ifdef CU_DEBUG
        if (mouseAxises.find(name) != mouseAxises.end()) {

            LogError(MOUSE_AXIS_ERR, name);
            return 0.0f;

        }
        if (axises.find(name) == axises.end()) {

            LogError(INVALID_AXIS_ERR, name);
            return 0.0f;

        }
#endif
#else
        CU_ASSERT(mouseAxises.find(name) == mouseAxises.end(), MOUSE_AXIS_ERR, name);
        CU_ASSERT(axises.find(name) != axises.end(), INVALID_AXIS_ERR, name);
#endif

        float ret = 0.0f;
        const Axis& axis = axises[name];

        if (Input::GetKeyState(axis.positive) == KeyState::Down) ret += axis.positiveValue;
        if (Input::GetKeyState(axis.negative) == KeyState::Down) ret += axis.negativeValue;

        return ret;

    }
    float GetMouseAxis(const std::string& name) {

        CUP_FUNCTION();

#if defined(CU_EDITOR) && defined(CU_DEBUG)
        if (axises.find(name) != axises.end()) {

            LogError(NORMAL_AXIS_ERR, name);
            return 0.0f;

        }
        if (mouseAxises.find(name) == mouseAxises.end()) {

            LogError(INVALID_AXIS_ERR, name);
            return 0.0f;

        }
#else
        CU_ASSERT(axises.find(name) == axises.end(), NORMAL_AXIS_ERR, name);
        CU_ASSERT(mouseAxises.find(name) != mouseAxises.end(), INVALID_AXIS_ERR, name);
#endif

        float ret = 0.0f;
        const MouseAxis& axis = mouseAxises[name];

        ret += axis.isX ? Input::GetCursorPosChange().x : Input::GetCursorPosChange().y;
        return ret;

    }

    void AddAxis(const std::string& axisName, KeyCode positive, KeyCode negative, float positiveValue, float negativeValue) {

        CUP_FUNCTION();

        axises[axisName] = Axis(positive, negative, positiveValue, negativeValue);

    }
    void AddMouseAxis(const std::string& axisName, bool isX, float positiveValue, float negativeValue) {

        CUP_FUNCTION();

        mouseAxises[axisName] = MouseAxis(isX, positiveValue, negativeValue);

    }

}
