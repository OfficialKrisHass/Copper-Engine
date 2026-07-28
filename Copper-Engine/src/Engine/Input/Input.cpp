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

    // Cursor

    CursorMode cursorMode = CursorMode::Normal;

    Vector2I prevCursorPosition;
    Vector2I cursorMotion;

    bool OnKeyPressed(const Event& e);
    bool OnKeyReleased(const Event& e);

    bool OnButtonPressed(const Event& e);
    bool OnButtonReleased(const Event& e);
    bool OnMouseMove(const Event& e);

    void Initialize(Window* win) {

        CUP_FUNCTION();

        VERIFY_STATE(EngineState::Initialization, "Initialize Input");
        CU_ASSERT(win != nullptr, "Invalid window passed to Input::Initialize().");

        window = win;

        window->GetKeyPressedEvent() += OnKeyPressed;
        window->GetKeyReleasedEvent() += OnKeyReleased;

        window->GetMouseButtonPressedEvent() += OnButtonPressed;
        window->GetMouseButtonReleasedEvent() += OnButtonReleased;
        window->GetMouseMoveEvent() += OnMouseMove;

#ifdef CU_DEBUG
        if (!pfd::settings::available())
#ifdef CU_LINUX
            LogError("Portable File Dialogs are not available! You might be missing these packages:\n\tKDE: KDialog\n\tGnome: Zenity/Matedialog/Qarma");
#elif CU_WINDOWS
            LogError("Portable File Dialogs are not available!");
#endif
#endif

        pfd::settings::verbose(false);

        prevCursorPosition = GetCursorPosition();

    }
    void Update() {

        CUP_FUNCTION();

        for (KeyCode key : keysToUpdate) {

            auto it = keyStates.find(key);
            switch (it->second) {

            case KeyState::Pressed: it->second = KeyState::Down; break;
            case KeyState::Released: it->second = KeyState::None; break;
            default: break;

            }

        }
        keysToUpdate.clear();

        cursorMotion = Vector2I::zero;

    }

    KeyState GetKeyState(KeyCode key) {

        CUP_FUNCTION();

        // The [] operator will insert (and return) KeyState::None if key is not present in the map, which is perfectly fine,
        // In the future when a KeyEvent is generated for key, it will already be present in the keyStates map because of this.

        return keyStates[key];

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

        cursorMotion = event.mouseCoords - prevCursorPosition;
        prevCursorPosition = event.mouseCoords;

        return true;

    }

    // Getters

    CursorMode GetCursorMode() { return cursorMode; }

    const Vector2I& GetCursorMotion() { return cursorMotion; }

    // Setters

    void SetCursorMode(CursorMode mode) {

        CUP_FUNCTION();

        cursorMode = mode;
        window->SetCursorMode(mode);

    }

}
