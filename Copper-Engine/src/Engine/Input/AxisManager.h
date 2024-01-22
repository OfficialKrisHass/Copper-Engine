#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Input/KeyCodes.h"
#include "Engine/Input/Input.h"

namespace Copper::Input {

	struct Axis {

		KeyCode positive = KeyCode::None;
		KeyCode negative = KeyCode::None;

		float positiveValue =  1.0f;
		float negativeValue = -1.0f;

	};

	struct MouseAxis {

		bool isX = true;

		float positiveValue =  1.0f;
		float negativeValue = -1.0f;

	};

	void InitializeAxisManager();

	float GetAxis(const std::string& axisName);

	void AddAxis(const std::string& axisName, KeyCode positive, KeyCode negative, float positiveValue = 1.0f, float negativeValue = -1.0f);
	void AddMouseAxis(const std::string& axisName, bool isX, float positiveValue = 1.0f, float negativeValue = -1.0f);

}