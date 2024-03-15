#include "Window.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace Launcher { extern void OnWindowClose(); }

namespace Launcher::Window {

	GLFWwindow* window = nullptr;

	void Create(uint32 width, uint32 height, const char* title) {

		if (!glfwInit()) {

			std::cout << "Failed to initialize GLFW!\n";
			return;

		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) { OnWindowClose(); });

	}
	void Update() {

		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	void Shutdown() {

		glfwDestroyWindow(window);
		glfwTerminate();

	}

	void* NativePointer() { return window; }

}