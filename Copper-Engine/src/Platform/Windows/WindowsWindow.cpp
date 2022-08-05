#include "cupch.h"
#include "Engine/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Copper {

	Window::Window(WindowData data) : data(data) {

		Log("Creating Window {0}: Width {1}, Height {2}", data.title, data.width, data.height);

		if (!glfwInit()) { LogError("Could not Initialize GLFW!"); }

		windowPtr = (void*) glfwCreateWindow(data.width, data.height, data.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent((GLFWwindow*) windowPtr);

	}

	void Window::Update() {

		glfwPollEvents();
		glfwSwapBuffers((GLFWwindow*) windowPtr);

	}

	void Window::Shutdown() {

		Log("Closing Window {0}", data.title);

		glfwDestroyWindow((GLFWwindow*) windowPtr);
		glfwTerminate();

	}

}