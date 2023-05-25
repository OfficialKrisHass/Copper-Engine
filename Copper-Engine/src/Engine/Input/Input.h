#pragma once

#include "Engine/Core/Core.h"

#include "KeyCodes.h"

namespace Copper::Input {

	//Input
	void Init();
	void Update();

	bool IsKey(KeyCode key);
	bool IsKeyDown(KeyCode key);
	bool IsKeyReleased(KeyCode key);

	bool IsButton(MouseCode button);

	//Setters
	void SetCursorVisible(bool visible);
	void SetCursorLocked(bool locked);
	void SetCursorPosition(float x, float y);

	void SetWindowTitle(const std::string& title);

	//Getters
	void GetCursorPosition(double* x, double* y);
	float GetCursorPosDifferenceX();
	float GetCursorPosDifferenceY();

	bool IsCursorLocked();
	bool IsCursorVisible();

	//Controls
	void WrapCursor();

}