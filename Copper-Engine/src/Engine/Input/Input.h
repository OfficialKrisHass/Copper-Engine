#pragma once

#include "Engine/Core/Core.h"

#include "KeyCodes.h"

namespace Copper::Input {
	
	enum MouseCode {
		
		Button1 = 0,
		Button2 = 1
		
	};

	//Input
	void Init();

	bool IsKey(KeyCode key);
	bool IsKeyDown(KeyCode key);
	bool IsKeyReleased(KeyCode key);

	bool IsButton(MouseCode button);

	//Setters
	void SetCursorVisible(bool visible);
	void SetCursorPosition(float x, float y);

	void SetWindowTitle(const std::string& title);

	//Getters
	void GetCursorPosition(double* x, double* y);
	float GetCursorPosDifferenceX();
	float GetCursorPosDifferenceY();

	//Controls
	void WrapCursor();

}