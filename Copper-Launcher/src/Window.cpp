#include "Window.h"

#include "LauncherApp.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Launcher::Window {

	GLFWwindow* window;

	void Initialize() {

		if (!glfwInit()) { std::cout << "GLFW Failed to Initialize!\n"; return; }

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(1280, 720, "Copper Launcher", nullptr, nullptr);
		if(!window) { std::cout << "GLFW Failed to Create the Window!\n"; return; }
		glfwMakeContextCurrent(window);

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) { OnWindowClose(); });

		if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) { std::cout << "GLAD Failed to Initialize!\n"; return; }

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glPointSize(10.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
	void Update() {

		glfwPollEvents();
		glfwSwapBuffers(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.18f, 0.18f, 0.18f, 1.0f);

	}
	void Shutdown() {

		glfwDestroyWindow(window);
		glfwTerminate();

	}

	void* Get() { return (void*) window; }

}