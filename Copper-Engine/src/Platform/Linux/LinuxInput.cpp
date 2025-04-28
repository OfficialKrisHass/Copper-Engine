#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Window.h"

#include <GLFW/glfw3.h>

#define GLFW_WINDOW(win) (GLFWwindow*) win.GetWindowPtr()

namespace Copper::Input {

    bool mouseVisible = true;
    bool mouseLocked = false;

    bool GetCursorVisible() { return mouseVisible; }
    bool GetCursorLocked() { return mouseLocked; }
    Vector2I GetCursorPosition() {

        CUP_FUNCTION();

        double x, y;
        glfwGetCursorPos(GLFW_WINDOW(GetWindow()), &x, &y);

        return Vector2I(static_cast<int32>(x), static_cast<int32>(y));

    }

    void SetCursorVisible(bool visible) {

        CUP_FUNCTION();

        glfwSetInputMode(GLFW_WINDOW(GetWindow()), GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
        mouseVisible = visible;

    }
    void SetCursorLocked(bool locked) {
        
        mouseLocked = locked;

    }
    void SetCursorPosition(int32 x, int32 y) {

        CUP_FUNCTION();

        glfwSetCursorPos(GLFW_WINDOW(GetWindow()), static_cast<double>(x), static_cast<double>(y));

    }

}
