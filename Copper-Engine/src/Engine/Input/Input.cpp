#include "cupch.h"
#include "Input.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/MouseEvent.h"

#include <portable-file-dialogs.h>

namespace Copper::Input {

    Window* window;

    // Key states

    std::unordered_map<KeyCode, KeyState> keyStates;
    std::vector<KeyCode> keysToUpdate;

    // Mouse

    Vector2I prevMousePos;
    Vector2I cursorPosChange;

    bool OnKeyPressed(const Event& e);
    bool OnKeyReleased(const Event& e);

    bool OnButtonPressed(const Event& e);
    bool OnButtonReleased(const Event& e);
    bool OnMouseMove(const Event& e);

    void Initialize(Window* win) {

        CUP_FUNCTION();

        VERIFY_STATE(EngineState::Initialization, "Initialize Input");
        window = win;

        window->GetKeyPressedEvent() += OnKeyPressed;
        window->GetKeyReleasedEvent() += OnKeyReleased;

        window->GetMouseButtonPressedEvent() += OnButtonPressed;
        window->GetMouseButtonReleasedEvent() += OnButtonReleased;
        window->GetMouseMoveEvent() += OnMouseMove;

        if (!pfd::settings::available())
#ifdef CU_LINUX
            LogError("Portable File Dialogs are not available! You might be missing these packages:\n\tKDE: KDialog\n\tGnome: Zenity/Matedialog/Qarma");
#elif CU_WINDOWS
            LogError("Portable File Dialogs are not available!");
#endif

        pfd::settings::verbose(false);

        prevMousePos = GetCursorPosition();

    }
    void Update() {

        CUP_FUNCTION();

        for (KeyCode key : keysToUpdate) {

            switch (keyStates.at(key)) {

            case KeyState::Pressed: keyStates[key] = KeyState::Down; break;
            case KeyState::Released: keyStates[key] = KeyState::None; break;
            default: break;

            }

        }
        keysToUpdate.clear();

        cursorPosChange = Vector2I::zero;

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

        cursorPosChange = event.mouseCoords - prevMousePos;
        if (GetCursorLocked()) {

            SetCursorPosition(prevMousePos.x, prevMousePos.y);
            return true;

        }

        prevMousePos = event.mouseCoords;
        return true;

    }

    const Vector2I& GetCursorPosChange() { return cursorPosChange; }

}
