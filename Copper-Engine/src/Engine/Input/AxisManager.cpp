#include "cupch.h"
#include "AxisManager.h"

namespace Copper::Input {

	std::unordered_map<std::string, Axis> axises;
	std::unordered_map<std::string, MouseAxis> mouseAxises;

	float GetAxis(const std::string& axisName) {

		bool isMouseAxis = mouseAxises.find(axisName) != mouseAxises.end();
		if (!isMouseAxis && axises.find(axisName) == axises.end()) {
			
			LogWarn("The axis {} doesn't exist, double check if the name you've provided is correct.");
			return 0.0f;
		
		}
		if (isMouseAxis) {

			float ret = 0.0f;
			MouseAxis axis = mouseAxises[axisName];

			ret += axis.isX ? Input::GetCursorPosDifferenceX() : Input::GetCursorPosDifferenceY();

			return ret;

		}

		float ret = 0.0f;
		Axis axis = axises[axisName];

		if (Input::IsKey(axis.positive)) ret += axis.positiveValue;
		if (Input::IsKey(axis.negative)) ret += axis.negativeValue;

		return ret;

	}

	void AddAxis(const std::string& axisName, KeyCode positive, KeyCode negative, float positiveValue, float negativeValue) {

		axises[axisName] = Axis {positive, negative, positiveValue, negativeValue};

	}
	void AddMouseAxis(const std::string& axisName, bool isX, float positiveValue, float negativeValue) {

		mouseAxises[axisName] = MouseAxis {isX, positiveValue, negativeValue};

	}

}