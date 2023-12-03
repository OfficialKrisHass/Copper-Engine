#include "cupch.h"
#include "Engine/Core/Window.h"

#include "Engine/Core/Engine.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Input/KeyCodes.h"
#include "Engine/Input/Input.h"

#include <GLFW/glfw3.h>

#define WINDOW (GLFWwindow*) windowPtr
#define GETWINDATA *(WindowData*) glfwGetWindowUserPointer(window)

namespace Copper {

	uint32_t windowCount = 0;

	Window::Window(const std::string& title, uint32_t width, uint32_t height, bool maximize) {

		data.title = title;

#ifdef CU_EDITOR // we cant verify the state in the editor because the editor creates the window before Engine Initialization
		if (windowCount == 0 && !glfwInit()) { LogError("Could not Initialize GLFW!"); }
#else
		if (windowCount == 0) {

			VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the main Window");
			if (!glfwInit()) { LogError("Could not Initialize GLFW"); }

		}
#endif
		windowCount++;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		windowPtr = (void*)glfwCreateWindow(width, height, data.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(WINDOW);

		glfwSetWindowUserPointer(WINDOW, &data);
		if (maximize) glfwMaximizeWindow(WINDOW);

		data.size = Size();

		SetupEvents();

	}
	void Window::Update() {

		glfwPollEvents();
		glfwSwapBuffers(WINDOW);

	}
	void Window::Shutdown() {

		glfwDestroyWindow(WINDOW);

		if (windowCount == 1) glfwTerminate();
		windowCount--;

	}

	void Window::SetAsCurrentContext() {

		glfwMakeContextCurrent(WINDOW);

	}

	void Window::SetupEvents() {

		glfwSetWindowCloseCallback(WINDOW, [](GLFWwindow* window) {

			WindowData& data = GETWINDATA;

			data.windowCloseEvent();

			});
		glfwSetWindowFocusCallback(WINDOW, [](GLFWwindow* window, int focused) {

			WindowData& data = GETWINDATA;

			data.windowFocusedEvent.focused = focused;
			data.windowFocusedEvent.Call();
			data.windowFocusedEvent.Clear();

			});
		glfwSetWindowSizeCallback(WINDOW, [](GLFWwindow* window, int width, int height) {

			WindowData& data = GETWINDATA;

			data.windowResizeEvent.width = width; data.size.x = width;
			data.windowResizeEvent.height = height; data.size.y = height;

			data.windowResizeEvent();

			});

		glfwSetKeyCallback(WINDOW, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData& data = GETWINDATA;

			switch (action) {

			case GLFW_PRESS:
			{

				data.keyPressedEvent.key = (KeyCode)key;
				data.keyPressedEvent.Call();
				data.keyPressedEvent.Clear();

				break;

			}
			case GLFW_REPEAT:
			{

				data.keyPressedEvent.key = (KeyCode)key;
				data.keyPressedEvent.Call();
				data.keyPressedEvent.Clear();

				break;

			}
			case GLFW_RELEASE:
			{

				data.keyReleasedEvent.key = (KeyCode)key;
				data.keyReleasedEvent.Call();
				data.keyReleasedEvent.Clear();

				break;

			}

			}

			});

		glfwSetCursorPosCallback(WINDOW, [](GLFWwindow* window, double x, double y) {

			WindowData& data = GETWINDATA;

			data.mouseMoveEvent.mouseCoords.x = (uint32_t) x;
			data.mouseMoveEvent.mouseCoords.y = (uint32_t) y;
			data.mouseMoveEvent();

			});

	}

	bool Window::IsKeyPressed(KeyCode keycode) const {

		return glfwGetKey(WINDOW, (int)keycode) == GLFW_PRESS;

	}
	bool Window::IsButtonPressed(MouseCode mousecode) const {

		return glfwGetMouseButton(WINDOW, (int)mousecode) == GLFW_PRESS;

	}

	uint32_t Window::Width() const {

		uint32_t ret;

		glfwGetWindowSize(WINDOW, (int*)&ret, nullptr);

		return ret;

	}
	uint32_t Window::Height() const {

		uint32_t ret;

		glfwGetWindowSize(WINDOW, nullptr, (int*)&ret);

		return ret;

	}
	UVector2I Window::Size() const {

		int x, y;
		glfwGetWindowSize(WINDOW, &x, &y);

		return UVector2I(x, y);

	}
	float Window::AspectRatio() const {

		UVector2I ret;

		glfwGetWindowSize(WINDOW, (int*)&ret.x, (int*)&ret.y);

		return static_cast<float>(ret.x) / ret.y;

	}

	float Window::Time() const {

		return (float)glfwGetTime();

	}

	void Window::SetSize(const UVector2I& size) {

		glfwSetWindowSize(WINDOW, size.x, size.y);
		data.size.x = size.x;
		data.size.y = size.y;

	}

	void Window::AddWindowCloseEventFunc(std::function<bool(const Event& e)> func) {

		data.windowCloseEvent += func;
		glfwSetWindowUserPointer(WINDOW, &data);

	}
	void Window::AddWindowFocusedEventFunc(std::function<bool(const Event& e)> func) {

		data.windowFocusedEvent += func;
		glfwSetWindowUserPointer(WINDOW, &data);

	}
	void Window::AddWindowResizeEventFunc(std::function<bool(const Event& e)> func) {

		data.windowResizeEvent += func;
		glfwSetWindowUserPointer(WINDOW, &data);

	}

	void Window::AddKeyPressedEventFunc(std::function<bool(const Event&)> func) {

		data.keyPressedEvent += func;
		glfwSetWindowUserPointer(WINDOW, &data);

	}
	void Window::AddKeyReleasedEventFunc(std::function<bool(const Event&)> func) {

		data.keyReleasedEvent += func;
		glfwSetWindowUserPointer(WINDOW, &data);

	}

	void Window::AddMouseMoveEventFunc(std::function<bool(const Event&)> func) {

		data.mouseMoveEvent += func;
		glfwSetWindowUserPointer(WINDOW, &data);

	}

}