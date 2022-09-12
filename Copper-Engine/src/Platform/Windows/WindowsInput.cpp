#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Engine.h"

#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>

namespace Copper {
	
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
		
		glfwSetInputMode(GetGLFWwindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
		
	}

	void SetCursorPosition(float x, float y) {

		glfwSetCursorPos(GetGLFWwindow, x, y);
		
	}
	
	//-------------------
	//------Getters------
	//-------------------
	void GetCursorPosition(double* x, double* y) {
		
		glfwGetCursorPos(GetGLFWwindow, x, y);
		
	}
	
}