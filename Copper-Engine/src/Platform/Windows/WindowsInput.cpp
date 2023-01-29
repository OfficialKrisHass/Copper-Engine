#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Engine.h"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>

namespace Copper::Input {
	
	std::unordered_map<KeyCode, std::pair<uint32_t, bool>> keys;

	bool OnKeyPressed(const Event& e);
	bool OnKeyReleased(const Event& e);

	void Init() {

		GetWindow().AddKeyPressedEventFunc(OnKeyPressed);
		GetWindow().AddKeyReleasedEventFunc(OnKeyReleased);

	}

	bool IsKey(KeyCode key) {
		
		if (keys[key].first == 0 || keys[key].second) return false;

		return true;

	}
	bool IsKeyDown(KeyCode key) {
		
		if (keys[key].first > 1 || keys[key].first == 0) return false;

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

		return false;

	}
	bool OnKeyReleased(const Event& e) {

		KeyCode keycode = ((KeyEvent*) &e)->key;
		keys[keycode].first = 0;
		keys[keycode].second = true;

		return false;

	}
	
	void SetCursorVisible(bool visible) {
		
		glfwSetInputMode(GetGLFWwindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		
	}
	void SetCursorPosition(float x, float y) {

		glfwSetCursorPos(GetGLFWwindow, x, y);
		
	}

	void SetWindowTitle(std::string title) {

		glfwSetWindowTitle(GetGLFWwindow, title.c_str());
		
	}
	
	void GetCursorPosition(double* x, double* y) {
		
		glfwGetCursorPos(GetGLFWwindow, x, y);
		
	}
	
}