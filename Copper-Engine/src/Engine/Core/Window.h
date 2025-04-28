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
        Window(const std::string& title, uint32 width, uint32 height, bool maximize = true) { Initialize(title, width, height, maximize); }

        void Initialize(const std::string& title, uint32 width, uint32 height, bool maximize = true);
        void Update();
        void Shutdown();

        void SetAsCurrentContext();

        inline void* GetWindowPtr() const { return windowPtr; }

        // Getters

        float GetTime() const;

        inline uint32 GetWidth() const { return data.size.x; }
        inline uint32 GetHeight() const { return data.size.y; }
        inline const UVector2I& GetSize() const { return data.size; }
        inline float GetAspectRatio() const { return static_cast<float>(data.size.x) / data.size.y; }

        inline const std::string& GetTitle() const { return data.title; }

        inline WindowCloseEvent& GetWindowCloseEvent() { return data.windowCloseEvent; };
        inline WindowFocusedEvent& GetWindowFocusedEvent() { return data.windowFocusedEvent; };
        inline WindowResizeEvent& GetWindowResizeEvent() { return data.windowResizeEvent; };

        inline KeyEvent& GetKeyPressedEvent() { return data.keyPressedEvent; }
        inline KeyEvent& GetKeyRepeatEvent() { return data.keyRepeatEvent; }
        inline KeyEvent& GetKeyReleasedEvent() { return data.keyReleasedEvent; }

        inline MouseEvent& GetMouseMoveEvent() { return data.mouseMoveEvent; }
        inline MouseEvent& GetMouseButtonPressedEvent() { return data.mouseButtonPressedEvent; }
        inline MouseEvent& GetMouseButtonReleasedEvent() { return data.mouseButtonReleasedEvent; }

        // Setters

        void SetSize(const UVector2I& size);

        void SetTitle(const std::string& title);

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
