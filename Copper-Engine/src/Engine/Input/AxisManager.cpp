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
        AddAxis("Keys_AD", KeyCode::D, KeyCode::A);

        AddMouseAxis("Mouse X", true);
        AddMouseAxis("Mouse Y", false);

    }

    float GetAxis(const std::string& name) {

        CUP_FUNCTION();

        CU_EDITOR_ASSERT_RETURN(mouseAxises.find(name) == mouseAxises.end(), 0.0f, NORMAL_AXIS_ERR, name);
        CU_EDITOR_ASSERT_RETURN(axises.find(name) != axises.end(), 0.0f, INVALID_AXIS_ERR, name);

        float ret = 0.0f;
        const Axis& axis = axises[name];

        if (Input::GetKeyState(axis.positive) == KeyState::Down) ret += axis.positiveValue;
        if (Input::GetKeyState(axis.negative) == KeyState::Down) ret += axis.negativeValue;

        return ret;

    }
    float GetMouseAxis(const std::string& name) {

        CUP_FUNCTION();

        CU_EDITOR_ASSERT_RETURN(axises.find(name) == axises.end(), 0.0f, NORMAL_AXIS_ERR, name);
        CU_EDITOR_ASSERT_RETURN(mouseAxises.find(name) != mouseAxises.end(), 0.0f, INVALID_AXIS_ERR, name);

        float ret = 0.0f;
        const MouseAxis& axis = mouseAxises[name];

        ret += axis.isX ? Input::GetCursorMotion().x : Input::GetCursorMotion().y;
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
