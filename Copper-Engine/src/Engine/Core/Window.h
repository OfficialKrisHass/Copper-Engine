#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Events/WindowEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Copper {

    struct WindowData {

        UVector2I size;
        std::string title;

        WindowCloseEvent windowCloseEvent;
        WindowFocusedEvent windowFocusedEvent;
        WindowResizeEvent windowResizeEvent;

        KeyEvent keyPressedEvent;
        KeyEvent keyRepeatEvent;
        KeyEvent keyReleasedEvent;

        MouseEvent mouseMoveEvent;
        MouseEvent mouseButtonPressedEvent;
        MouseEvent mouseButtonReleasedEvent;

        WindowData() = default;

    };

    class Window {

    public:
        Window() = default;
        Window(const std::string& title, uint32 width, uint32 height, bool maximize = true);

        void Update();
        void Shutdown();

        void SetAsCurrentContext();

        inline void* GetWindowPtr() const { return windowPtr; }

        // Getters

        inline uint32 GetWidth() const { return data.size.x; }
        inline uint32 GetHeight() const { return data.size.y; }
        inline const UVector2I& GetSize() const { return data.size; }
        inline float GetAspectRatio() const { return static_cast<float>(data.size.x) / data.size.y; }

        float GetTime() const;

        // Setters

        void SetSize(const UVector2I& size);

        // Events

        void AddWindowCloseEventFunc(std::function<bool(const Event&)> func);
        void AddWindowFocusedEventFunc(std::function<bool(const Event&)> func);
        void AddWindowResizeEventFunc(std::function<bool(const Event&)> func);

        void AddKeyPressedEventFunc(std::function<bool(const Event&)> func);
        void AddKeyRepeatEventFunc(std::function<bool(const Event&)> func);
        void AddKeyReleasedEventFunc(std::function<bool(const Event&)> func);

        void AddMouseMoveEventFunc(std::function<bool(const Event&)> func);
        void AddMouseButtonPressedEventFunc(std::function<bool(const Event&)> func);
        void AddMouseButtonReleasedEventFunc(std::function<bool(const Event&)> func);

    private:
        WindowData data;
        void* windowPtr = nullptr;

        void SetupEvents();

    };

    Window& GetWindow();
    UVector2I GetWindowSize();
    float GetWindowAspectRatio();

    void SetMainWindowAsCurrent();
    void SetWindowSize(const UVector2I& size);

}
