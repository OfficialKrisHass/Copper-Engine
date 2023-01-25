#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Input/KeyCodes.h"
#include "Engine/Input/Input.h"

namespace Copper::Input {

	struct Axis {

		KeyCode positive;
		KeyCode negative;

		float positiveValue =  1.0f;
		float negativeValue = -1.0f;

	};

	float GetAxis(const std::string& axisName);

	void AddAxis(const std::string& axisName, KeyCode positive, KeyCode negative, float positiveValue = 1.0f, float negativeValue = -1.0f);

}