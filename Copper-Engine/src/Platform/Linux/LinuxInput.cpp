#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/MouseEvent.h"

#include <GLFW/glfw3.h>
#include <portable-file-dialogs.h>

#ifdef CU_EDITOR
extern Copper::UVector2I GetViewportCentre();
#endif

#define GLFW_WINDOW(win) (GLFWwindow*) win->GetWindowPtr()

namespace Copper::Input {

    Window* window;

    // Key states

    std::unordered_map<KeyCode, KeyState> keyStates;
    std::vector<KeyCode> keysToUpdate;

    // Mouse

    bool mouseVisible = true;
    bool mouseLocked = false;
    bool firstMouseLockedFrame = true;

    UVector2I prevMousePos;
    Vector2 mousePosDiference;

    bool OnKeyPressed(const Event& e);
    bool OnKeyReleased(const Event& e);

    bool OnButtonPressed(const Event& e);
    bool OnButtonReleased(const Event& e);
    bool OnMouseMove(const Event& e);

    void Initialize(Window* win) {

        CUP_FUNCTION();

        VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize Input");
        window = win;

        window->AddKeyPressedEventFunc(OnKeyPressed);
        window->AddKeyReleasedEventFunc(OnKeyReleased);

        window->AddMouseButtonPressedEventFunc(OnButtonPressed);
        window->AddMouseButtonReleasedEventFunc(OnButtonReleased);
        window->AddMouseMoveEventFunc(OnMouseMove);

        if (!pfd::settings::available())
#ifdef CU_LINUX
            LogError("Portable File Dialogs are not available! You might be missing these packages:\n\tKDE: KDialog\n\tGnome: Zenity/Matedialog/Qarma");
#elif CU_WINDOWS
            LogError("Portable File Dialogs are not available!");
#endif

        pfd::settings::verbose(false);

    }
    void Update() {

        CUP_FUNCTION();

        mousePosDiference = Vector2::zero;

        for (KeyCode key : keysToUpdate) {

            switch (keyStates.at(key)) {

            case KeyState::Pressed: keyStates[key] = KeyState::Down; break;
            case KeyState::Released: keyStates[key] = KeyState::None; break;
            default: break;

            }

        }
        keysToUpdate.clear();

    }

    KeyState GetKeyState(KeyCode key) {

        CUP_FUNCTION();

        if (keyStates.find(key) == keyStates.end()) return KeyState::None;
        return keyStates.at(key);

    }

    bool OnKeyPressed(const Event& e) {

        CUP_FUNCTION();

        const KeyEvent& event = static_cast<const KeyEvent&>(e);

        keyStates[event.key] = KeyState::Pressed;
        keysToUpdate.push_back(event.key);

        return true;

    }
    bool OnKeyReleased(const Event& e) {

        CUP_FUNCTION();

        const KeyEvent& event = static_cast<const KeyEvent&>(e);

        keyStates[event.key] = KeyState::Released;
        keysToUpdate.push_back(event.key);

        return true;

    }

    bool OnButtonPressed(const Event& e) {

        CUP_FUNCTION();

        const MouseEvent& event = static_cast<const MouseEvent&>(e);

        keyStates[event.button] = KeyState::Pressed;
        keysToUpdate.push_back(event.button);

        return true;

    }
    bool OnButtonReleased(const Event& e) {

        CUP_FUNCTION();

        const MouseEvent& event = static_cast<const MouseEvent&>(e);

        keyStates[event.button] = KeyState::Released;
        keysToUpdate.push_back(event.button);

        return true;

    }
    bool OnMouseMove(const Event& e) {

        CUP_FUNCTION();

        const MouseEvent& event = static_cast<const MouseEvent&>(e);

        if (!mouseLocked) {

            UVector2I diferenceFull = prevMousePos - event.mouseCoords;
            mousePosDiference.x = -((float) diferenceFull.x / (float) GetWindow().GetWidth());
            mousePosDiference.y = -((float) diferenceFull.y / (float) GetWindow().GetHeight());

            prevMousePos = event.mouseCoords;

            return true;

        }

        UVector2I centre;
#ifdef CU_EDITOR
        centre = GetViewportCentre();
#else
        centre = GetWindowSize() / 2;
#endif

        if (event.mouseCoords == centre) return true;
        if (!firstMouseLockedFrame) {

            mousePosDiference.x = ((float) event.mouseCoords.x - centre.x) / GetWindowSize().x;
            mousePosDiference.y = ((float) event.mouseCoords.y - centre.y) / GetWindowSize().y;

        }
        else
            firstMouseLockedFrame = false;

        prevMousePos = event.mouseCoords;

        SetCursorPosition((float) centre.x, (float) centre.y);

        return true;

    }

    void SetCursorVisible(bool visible) {

        CUP_FUNCTION();

        glfwSetInputMode(GLFW_WINDOW(window), GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
        mouseVisible = visible;

    }
    void SetCursorLocked(bool locked) {

        CUP_FUNCTION();

        mouseLocked = locked;
        if (locked)
            firstMouseLockedFrame = true;

    }
    void SetCursorPosition(float x, float y) {

        CUP_FUNCTION();

        glfwSetCursorPos(GLFW_WINDOW(window), x, y);

    }

    void SetWindowTitle(const std::string& title) {

        CUP_FUNCTION();

        glfwSetWindowTitle(GLFW_WINDOW(window), title.c_str());

    }

    void GetCursorPosition(double* x, double* y) {

        CUP_FUNCTION();

        glfwGetCursorPos(GLFW_WINDOW(window), x, y);

    }

    float GetCursorPosDifferenceX() { return mousePosDiference.x; }
    float GetCursorPosDifferenceY() { return mousePosDiference.y; }

    bool IsCursorLocked() { return mouseLocked; }
    bool IsCursorVisible() { return mouseVisible; }

}
