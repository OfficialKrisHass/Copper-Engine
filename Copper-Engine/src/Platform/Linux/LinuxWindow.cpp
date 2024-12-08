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

    uint32 windowCount = 0;

    Window::Window(const std::string& title, uint32 width, uint32 height, bool maximize) {

        CUP_FUNCTION();

        data.title = title;

        if (windowCount == 0) {

#ifndef CU_EDITOR
            VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the main Window"); 
#endif
            if (!glfwInit()) {

                LogError("Could not initialize GLFW");
                exit(-1);

            }

        }

        windowCount++;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        windowPtr = (void*) glfwCreateWindow(width, height, data.title.c_str(), NULL, NULL);
        CU_ASSERT(windowPtr, "Could not create GLFW window");

        glfwMakeContextCurrent(WINDOW);
        glfwSetWindowUserPointer(WINDOW, &data);
        if (maximize)
            glfwMaximizeWindow(WINDOW);

        glfwGetWindowSize(WINDOW, (int32*) &data.size.x, (int32*) &data.size.y);

        SetupEvents();

    }
    void Window::Update() {

        CUP_FUNCTION();

        glfwPollEvents();
        glfwSwapBuffers(WINDOW);

    }
    void Window::Shutdown() {

        CUP_FUNCTION();

        glfwDestroyWindow(WINDOW);

        if (windowCount == 1)
            glfwTerminate();
        windowCount--;

    }

    void Window::SetAsCurrentContext() {

        CUP_FUNCTION();

        glfwMakeContextCurrent(WINDOW);

    }

    void Window::SetupEvents() {

        CUP_FUNCTION();

        glfwSetWindowCloseCallback(WINDOW, [](GLFWwindow* window) {

            WindowData& data = GETWINDATA;

            data.windowCloseEvent();

        });
        glfwSetWindowFocusCallback(WINDOW, [](GLFWwindow* window, int32 focused) {

            WindowData& data = GETWINDATA;

            data.windowFocusedEvent.focused = focused;
            data.windowFocusedEvent();

        });
        glfwSetWindowSizeCallback(WINDOW, [](GLFWwindow* window, int32 width, int32 height) {

            WindowData& data = GETWINDATA;

            data.size.x = width;
            data.size.y = height;

            data.windowResizeEvent.width = width;
            data.windowResizeEvent.height = height;

            data.windowResizeEvent();

        });

        glfwSetKeyCallback(WINDOW, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {

            WindowData& data = GETWINDATA;

            switch (action) {

            case GLFW_PRESS:
            {

                data.keyPressedEvent.key = (KeyCode)key;
                data.keyPressedEvent();

                break;

            }
            case GLFW_REPEAT:
            {

                data.keyPressedEvent.key = (KeyCode)key;
                data.keyPressedEvent();

                break;

            }
            case GLFW_RELEASE:
            {

                data.keyReleasedEvent.key = (KeyCode)key;
                data.keyReleasedEvent();

                break;

            }

            }

        });

        glfwSetCursorPosCallback(WINDOW, [](GLFWwindow* window, double x, double y) {

            WindowData& data = GETWINDATA;

            data.mouseMoveEvent.mouseCoords.x = (uint32) x;
            data.mouseMoveEvent.mouseCoords.y = (uint32) y;
            data.mouseMoveEvent();

        });

    }

    float Window::GetTime() const {

        CUP_FUNCTION();

        return (float) glfwGetTime();

    }

    void Window::SetSize(const UVector2I& size) {

        CUP_FUNCTION();

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
