#include "cupch.h"
#include "AxisManager.h"

namespace Copper::Input {

	std::unordered_map<std::string, Axis> axises;

	float GetAxis(const std::string& axisName) {

		if (axises.find(axisName) == axises.end()) {
			
			LogWarn("The axis {} doesn't exist, double check if the name you've provided is correct.");
			return 0.0f;
		
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

}