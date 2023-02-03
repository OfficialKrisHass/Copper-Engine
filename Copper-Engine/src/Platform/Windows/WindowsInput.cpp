#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Engine.h"

#include "Engine/Events/MouseEvent.h"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>

namespace Copper::Input {
	
	std::unordered_map<KeyCode, std::pair<uint32_t, bool>> keys;

	Vector2 prevMousePos;
	Vector2 mousePosDiference;

	bool OnKeyPressed(const Event& e);
	bool OnKeyReleased(const Event& e);

	bool OnMouseMove(const Event& e);

	void Init() {

		CHECK((GetEngineState() == EngineState::Initialization), "Cannot Initialize Input, current Engine State is: {}", EngineStateToString(GetEngineState()))

		GetWindow().AddKeyPressedEventFunc(OnKeyPressed);
		GetWindow().AddKeyReleasedEventFunc(OnKeyReleased);

		GetWindow().AddMouseMoveEventFunc(OnMouseMove);

	}

	bool IsKey(KeyCode key) {
		
		if (keys[key].first == 0) return false;

		return true;

	}
	bool IsKeyDown(KeyCode key) {
		
		if (keys[key].first != 1) return false;

		keys[key].first++;
		return true;

	}
	bool IsKeyReleased(KeyCode key) {
		
		if (!keys[key].second) return false;

		keys[key].second = false;
		return true;

	}
	
	bool IsButton(MouseCode button) {
		
		return glfwGetMouseButton(GetGLFWwindow, button) == GLFW_PRESS ? true : false;

	}

	bool OnKeyPressed(const Event& e) {

		KeyCode keycode = ((KeyEvent*) &e)->key;
		keys[keycode].first++;
		keys[keycode].second = false;

		return true;

	}
	bool OnKeyReleased(const Event& e) {

		KeyCode keycode = ((KeyEvent*) &e)->key;
		keys[keycode].first = 0;
		keys[keycode].second = true;

		return true;

	}

	bool OnMouseMove(const Event& e) {

		MouseMoveEvent& event = *((MouseMoveEvent*) &e);

		Vector2I diferenceFull = prevMousePos - event.mouseCoords;
		mousePosDiference.x = (float) diferenceFull.x / (float) GetWindow().Width();
		mousePosDiference.y = (float) diferenceFull.y / (float) GetWindow().Height();

		prevMousePos = event.mouseCoords;

		return true;

	}
	
	void SetCursorVisible(bool visible) {
		
		glfwSetInputMode(GetGLFWwindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		
	}
	void SetCursorPosition(float x, float y) {

		glfwSetCursorPos(GetGLFWwindow, x, y);
		
	}

	void SetWindowTitle(const std::string& title) {

		glfwSetWindowTitle(GetGLFWwindow, title.c_str());
		
	}
	
	void GetCursorPosition(double* x, double* y) {
		
		glfwGetCursorPos(GetGLFWwindow, x, y);
		
	}

	float GetCursorPosDifferenceX() { return mousePosDiference.x; }
	float GetCursorPosDifferenceY() { return mousePosDiference.y; }
	
}