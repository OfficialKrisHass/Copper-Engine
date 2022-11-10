#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Engine.h"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>

namespace Copper::Input {
	
	//------------------------------------
	//------Keyboard and Mouse Input------
	//------------------------------------
	bool IsKey(KeyCode key) {

		return glfwGetKey(GetGLFWwindow, key) == GLFW_PRESS ? true : false;

	}
	
	bool IsButton(MouseCode button) {
		
		return glfwGetMouseButton(GetGLFWwindow, button) == GLFW_PRESS ? true : false;

	}
	
	//-------------------
	//------Setters------
	//-------------------
	void SetCursorVisible(bool visible) {
		
		glfwSetInputMode(GetGLFWwindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		
	}

	void SetCursorPosition(float x, float y) {

		glfwSetCursorPos(GetGLFWwindow, x, y);
		
	}

	void SetWindowTitle(std::string title) {

		glfwSetWindowTitle(GetGLFWwindow, title.c_str());
		
	}
	
	//-------------------
	//------Getters------
	//-------------------
	void GetCursorPosition(double* x, double* y) {
		
		glfwGetCursorPos(GetGLFWwindow, x, y);
		
	}

	//---------------------------------
	//------Controlling functions------
	//---------------------------------
	void WrapCursor() {

		float rightEdgeX = GetWindow().Width() - 5;
		double mouseX, mouseY;
		GetCursorPosition(&mouseX, &mouseY);

		if(mouseX <= 5) { SetCursorPosition(rightEdgeX, mouseY); }
		if(mouseY >= rightEdgeX) { SetCursorPosition(5, mouseY); }
		
	}
	
}