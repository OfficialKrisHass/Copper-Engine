#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Events/WindowEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Copper {

    struct WindowData {

        UVector2I size;
        std::string title;

        Event windowCloseEvent;
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
        Window(const std::string& title, uint32 width, uint32 height, bool maximize = true) { Create(title, width, height, maximize); }

        void Create(const std::string& title, uint32 width, uint32 height, bool maximize = true);
        void Update();
        void Shutdown();

        static void InitializeBackend();
        static void ShutdownBackend();

        void SetAsCurrentContext();

        inline void* GetWindowPtr() const { return m_windowPtr; }

        // Getters

        float GetTime() const;

        inline uint32 GetWidth() const { return m_data.size.x; }
        inline uint32 GetHeight() const { return m_data.size.y; }
        inline const UVector2I& GetSize() const { return m_data.size; }
        inline float GetAspectRatio() const { return static_cast<float>(m_data.size.x) / m_data.size.y; }

        inline const std::string& GetTitle() const { return m_data.title; }

        // Setters

        void SetWidth(uint32 value);
        void SetHeight(uint32 value);
        void SetSize(const UVector2I& size);

        void SetTitle(const std::string& title);

        // Events

        inline Event& GetWindowCloseEvent() { return m_data.windowCloseEvent; };
        inline WindowFocusedEvent& GetWindowFocusedEvent() { return m_data.windowFocusedEvent; };
        inline WindowResizeEvent& GetWindowResizeEvent() { return m_data.windowResizeEvent; };

        inline KeyEvent& GetKeyPressedEvent() { return m_data.keyPressedEvent; }
        inline KeyEvent& GetKeyRepeatEvent() { return m_data.keyRepeatEvent; }
        inline KeyEvent& GetKeyReleasedEvent() { return m_data.keyReleasedEvent; }

        inline MouseEvent& GetMouseMoveEvent() { return m_data.mouseMoveEvent; }
        inline MouseEvent& GetMouseButtonPressedEvent() { return m_data.mouseButtonPressedEvent; }
        inline MouseEvent& GetMouseButtonReleasedEvent() { return m_data.mouseButtonReleasedEvent; }

    private:
        WindowData m_data;
        void* m_windowPtr = nullptr;

        void SetupEvents();

    };

    Window& GetWindow();
    UVector2I GetWindowSize();
    float GetWindowAspectRatio();

    void SetMainWindowAsCurrent();
    void SetWindowSize(const UVector2I& size);

}
