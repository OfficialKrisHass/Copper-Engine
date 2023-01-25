#pragma once

#include "Engine/Core/Core.h"

#include "KeyCodes.h"

namespace Copper::Input {
	
	enum MouseCode {
		
		Button1 = 0,
		Button2 = 1
		
	};

	//Input
	bool IsKey(KeyCode key);
	bool IsButton(MouseCode button);

	//Setters
	void SetCursorVisible(bool visible);
	void SetCursorPosition(float x, float y);

	void SetWindowTitle(std::string title);

	void SetAcceptInputDuringRuntime(bool value);

	//Getters
	void GetCursorPosition(double* x, double* y);

	//Controls
	void WrapCursor();

}