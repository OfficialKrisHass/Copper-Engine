#include "cupch.h"
#include "Engine/Core/Window.h"

#include "Engine/Core/Engine.h"

#include "Engine/Input/KeyCodes.h"
#include "Engine/Input/Input.h"
#include "Engine/Input/Popup.h"

#include <GLFW/glfw3.h>

#define WINDOW static_cast<GLFWwindow*>(m_windowPtr)
#define GETWINDATA static_cast<WindowData*>(glfwGetWindowUserPointer(window))

namespace Copper {

    void Window::Create(const std::string& title, uint32 width, uint32 height, bool maximize) {

        CUP_FUNCTION();

        if (m_windowPtr != nullptr) {

            LogError("Window {} was already created, please call Shutdown() first", title);
            return;

        }

        m_data.title = title;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_windowPtr = (void*) glfwCreateWindow(width, height, m_data.title.c_str(), NULL, NULL);
        CU_ASSERT(m_windowPtr != nullptr, "Could not create the GLFW window for window {}", m_data.title);

        glfwMakeContextCurrent(WINDOW);
        glfwSetWindowUserPointer(WINDOW, &m_data);
        if (maximize)
            glfwMaximizeWindow(WINDOW);

        glfwGetWindowSize(WINDOW, (int32*) &m_data.size.x, (int32*) &m_data.size.y);

        SetupEvents();

        Log("Created window {}, size: {}.", m_data.title, m_data.size);

    }
    void Window::Update() {

        CUP_FUNCTION();

        CU_ASSERT(m_windowPtr != nullptr, "Invalid window pointer on window '{}'", m_data.title);

        glfwPollEvents();
        glfwSwapBuffers(WINDOW);

    }
    void Window::Shutdown() {

        CUP_FUNCTION();

        glfwDestroyWindow(WINDOW);

    }

    void Window::InitializeBackend() {

        CUP_FUNCTION();

#ifdef CU_EDITOR
        VERIFY_STATE(EngineState::Entry, "Initialize GLFW");
#else
        VERIFY_STATE(EngineState::Initialization, "Initialize GLFW");
#endif

        if (glfwInit()) return;

        Input::ErrorPopup("GLFW error", "Failed to initialize GLFW.");
        exit(-1);

    }
    void Window::ShutdownBackend() {

        CUP_FUNCTION();

        VERIFY_STATE(EngineState::Shutdown, "Shutdown GLFW");

        glfwTerminate();

    }

    void Window::SetAsCurrentContext() {

        CUP_FUNCTION();

        glfwMakeContextCurrent(WINDOW);

    }

    void Window::SetupEvents() {

        CUP_FUNCTION();

        glfwSetWindowCloseCallback(WINDOW, [](GLFWwindow* window) {

            WindowData* data = GETWINDATA;

            data->windowCloseEvent();

        });
        glfwSetWindowFocusCallback(WINDOW, [](GLFWwindow* window, int32 focused) {

            WindowData* data = GETWINDATA;

            data->windowFocusedEvent.focused = focused;
            data->windowFocusedEvent();

        });
        glfwSetWindowSizeCallback(WINDOW, [](GLFWwindow* window, int32 width, int32 height) {

            WindowData* data = GETWINDATA;

            data->size.x = width;
            data->size.y = height;

            data->windowResizeEvent.width = width;
            data->windowResizeEvent.height = height;

            data->windowResizeEvent();

        });

        glfwSetKeyCallback(WINDOW, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {

            WindowData* data = GETWINDATA;

            switch (action) {

            case GLFW_PRESS:
            {

                data->keyPressedEvent.key = (KeyCode) key;
                data->keyPressedEvent();

                break;

            }
            case GLFW_REPEAT:
            {

                data->keyRepeatEvent.key = (KeyCode) key;
                data->keyRepeatEvent();

                break;

            }
            case GLFW_RELEASE:
            {

                data->keyReleasedEvent.key = (KeyCode) key;
                data->keyReleasedEvent();

                break;

            }

            }

        });
        glfwSetMouseButtonCallback(WINDOW, [](GLFWwindow* window, int32 button, int32 action, int32 mods) {

            WindowData* data = GETWINDATA;

            uint16 keycode = static_cast<uint16>(KeyCode::Mouse0) + button;

            switch (action) {

            case GLFW_PRESS: {

                data->mouseButtonPressedEvent.button = static_cast<KeyCode>(keycode);
                data->mouseButtonPressedEvent();

                break;

            }
            case GLFW_RELEASE: {

                data->mouseButtonReleasedEvent.button = static_cast<KeyCode>(keycode);
                data->mouseButtonReleasedEvent();

                break;

            }
            default: break;

            }

        });

        glfwSetCursorPosCallback(WINDOW, [](GLFWwindow* window, double x, double y) {

            WindowData* data = GETWINDATA;

            data->mouseMoveEvent.mouseCoords.x = (int32) x;
            data->mouseMoveEvent.mouseCoords.y = (int32) y;
            data->mouseMoveEvent();

        });

    }

    float Window::GetTime() const {

        CUP_FUNCTION();

        return (float) glfwGetTime();

    }

    void Window::SetWidth(uint32 value) {

        CUP_FUNCTION();

        glfwSetWindowSize(WINDOW, value, m_data.size.y);
        m_data.size.x = value;

    }
    void Window::SetHeight(uint32 value) {

        CUP_FUNCTION();

        glfwSetWindowSize(WINDOW, m_data.size.x, value);
        m_data.size.y = value;

    }
    void Window::SetSize(const UVector2I& size) {

        CUP_FUNCTION();

        glfwSetWindowSize(WINDOW, size.x, size.y);
        m_data.size.x = size.x;
        m_data.size.y = size.y;

    }

    void Window::SetTitle(const std::string& title) {

        CUP_FUNCTION();

        glfwSetWindowTitle(WINDOW, title.c_str());
        m_data.title = title;

    }

}
