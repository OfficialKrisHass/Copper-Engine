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

    void Window::Initialize(const std::string& title, uint32 width, uint32 height, bool maximize) {

        CUP_FUNCTION();

        if (windowPtr != nullptr) {

            LogError("Window {} is already initialized, please call Shutdown() first", title);
            return;

        }

        data.title = title;

        if (windowCount == 0) {

#ifndef CU_EDITOR
            VERIFY_STATE(EngineState::Initialization, "Initialize the main Window");
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

        windowPtr = (void*)glfwCreateWindow(width, height, data.title.c_str(), NULL, NULL);
        CU_ASSERT(windowPtr, "Could not create GLFW window");

        glfwMakeContextCurrent(WINDOW);
        glfwSetWindowUserPointer(WINDOW, &data);
        if (maximize)
            glfwMaximizeWindow(WINDOW);

        glfwGetWindowSize(WINDOW, (int32*)&data.size.x, (int32*)&data.size.y);

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

                data.keyRepeatEvent.key = (KeyCode)key;
                data.keyRepeatEvent();

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

        glfwSetMouseButtonCallback(WINDOW, [](GLFWwindow* window, int32 button, int32 action, int32 mods) {

            WindowData& data = GETWINDATA;
            uint16 keycode = static_cast<uint16>(KeyCode::Mouse0) + button;

            switch (action) {

            case GLFW_PRESS: {

                data.mouseButtonPressedEvent.button = static_cast<KeyCode>(keycode);
                data.mouseButtonPressedEvent();

                break;

            }
            case GLFW_RELEASE: {
 
                data.mouseButtonReleasedEvent.button = static_cast<KeyCode>(keycode);
                data.mouseButtonReleasedEvent();

                break;

            }
            default: break;

            }

            });

        glfwSetCursorPosCallback(WINDOW, [](GLFWwindow* window, double x, double y) {

            WindowData& data = GETWINDATA;

            data.mouseMoveEvent.mouseCoords.x = (int32)x;
            data.mouseMoveEvent.mouseCoords.y = (int32)y;
            data.mouseMoveEvent();

            });

    }

    float Window::GetTime() const {

        CUP_FUNCTION();

        return (float)glfwGetTime();

    }

    void Window::SetSize(const UVector2I& size) {

        CUP_FUNCTION();

        glfwSetWindowSize(WINDOW, size.x, size.y);
        data.size.x = size.x;
        data.size.y = size.y;

    }

}
