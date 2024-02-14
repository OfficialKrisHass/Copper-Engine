#include "cupch.h"
#include "AxisManager.h"

#include "Engine/Core/Engine.h"

namespace Copper::Input {

	std::unordered_map<std::string, Axis> axises;
	std::unordered_map<std::string, MouseAxis> mouseAxises;

	void InitializeAxisManager() {
		
		CUP_FUNCTION();

		VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the Axis Manager");

		AddAxis("Keys_WS", KeyCode::W, KeyCode::S);
		AddAxis("Keys_DA", KeyCode::D, KeyCode::A);

		AddMouseAxis("Mouse X", true);
		AddMouseAxis("Mouse Y", false);

	}

	float GetAxis(const std::string& axisName) {

		CUP_FUNCTION();

		bool isMouseAxis = mouseAxises.find(axisName) != mouseAxises.end();
		if (!isMouseAxis && axises.find(axisName) == axises.end()) {
			
			LogWarn("The axis ({}) doesn't exist, double check if the name you've provided is correct.", axisName);
			return 0.0f;
		
		}

		// Mouse Axis

		if (isMouseAxis) {

			float ret = 0.0f;
			MouseAxis axis = mouseAxises[axisName];

			ret += axis.isX ? Input::GetCursorPosDifferenceX() : Input::GetCursorPosDifferenceY();

			return ret;

		}

		// Keyboard axis

		float ret = 0.0f;
		Axis axis = axises[axisName];

		if (Input::IsKey(axis.positive)) ret += axis.positiveValue;
		if (Input::IsKey(axis.negative)) ret += axis.negativeValue;

		return ret;

	}

	void AddAxis(const std::string& axisName, KeyCode positive, KeyCode negative, float positiveValue, float negativeValue) {

		CUP_FUNCTION();

		axises[axisName] = Axis {positive, negative, positiveValue, negativeValue};

	}
	void AddMouseAxis(const std::string& axisName, bool isX, float positiveValue, float negativeValue) {

		CUP_FUNCTION();

		mouseAxises[axisName] = MouseAxis {isX, positiveValue, negativeValue};

	}

}