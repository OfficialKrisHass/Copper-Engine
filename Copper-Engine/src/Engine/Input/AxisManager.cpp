#include "cupch.h"
#include "AxisManager.h"

#include "Engine/Core/Engine.h"

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

        bool isMouseAxis = mouseAxises.find(name) != mouseAxises.end();

        if (isMouseAxis)
            return GetMouseAxis(name);
        else if (axises.find(name) == axises.end()) {

            LogWarn("The axis '{}' does not exist, double check if the name you've provided is correct", name);
            return 0.0f;

        }

        float ret = 0.0f;
        const Axis& axis = axises[name];

        if (Input::GetKeyState(axis.positive) == KeyState::Down) ret += axis.positiveValue;
        if (Input::GetKeyState(axis.negative) == KeyState::Down) ret += axis.negativeValue;

        return ret;

    }
    float GetMouseAxis(const std::string& name) {

        CUP_FUNCTION();

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
