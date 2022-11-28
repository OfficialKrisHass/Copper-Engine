#include "cupch.h"
#include "Engine/Core/Window.h"

#include "Engine/Core/Engine.h"
#include "Engine/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

#define WINDOW (GLFWwindow*) windowPtr
#define GETWINDATA *(WindowData*) glfwGetWindowUserPointer(window)

namespace Copper {

	Window::Window(WindowData winData) {

		data.title = winData.title;
		data.width = winData.width;
		data.height = winData.height;
		
		data.wResE = WindowResizeEvent(data.width, data.height);
		data.wClsE = WindowCloseEvent();

		data.kPrsE = KeyPresedEvent();

		if (!glfwInit()) { LogError("Could not Initialize GLFW!"); }

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		windowPtr = (void*) glfwCreateWindow(data.width, data.height, data.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(WINDOW);

		//Renderer::Initialize();
		RendererAPI::Initialize();

		data.wResE += OnWindowResize;
		data.wClsE += OnWindowClose;
		data.kPrsE += OnKeyPressed;

		glfwSetWindowUserPointer(WINDOW, &data);
		glfwMaximizeWindow(WINDOW);

		glfwSetWindowSizeCallback(WINDOW, [](GLFWwindow* window, int width, int height) {

			WindowData& data = GETWINDATA;

			data.wResE.width = width; data.width = width;
			data.wResE.height = height; data.height = height;

			data.wResE.Call();
			data.wResE.Clear();

		});
		glfwSetWindowCloseCallback(WINDOW, [](GLFWwindow* window) {

			WindowData& data = GETWINDATA;

			data.wClsE.Call();
			data.wClsE.Clear();

		});
		glfwSetKeyCallback(WINDOW, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData& data = GETWINDATA;

			switch (action) {

				case GLFW_PRESS: {

					data.kPrsE.key = (KeyCode) key;

					data.kPrsE.Call();
					data.kPrsE.Clear();

				}

			}

		});

	}

	void Window::Update() {

		glfwPollEvents();
		glfwSwapBuffers(WINDOW);

	}

	void Window::Shutdown() {

		glfwDestroyWindow(WINDOW);
		glfwTerminate();

	}

}