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
		KeyEvent keyReleasedEvent;

		MouseMoveEvent mouseMoveEvent;

		WindowData() = default;

	};

	class Window {

	public:
		Window() = default;
		Window(const std::string& title, uint32_t width, uint32_t height, bool maximize = true);

		void Update();
		void Shutdown();

		void SetAsCurrentContext();

		inline void* GetWindowPtr() const { return windowPtr; }

		uint32_t Width() const;
		uint32_t Height() const;
		UVector2I Size() const;
		float AspectRatio() const;

		float Time() const;

		void SetSize(const UVector2I& size);

		void AddWindowCloseEventFunc(std::function<bool(const Event&)> func);
		void AddWindowFocusedEventFunc(std::function<bool(const Event&)> func);
		void AddWindowResizeEventFunc(std::function<bool(const Event&)> func);

		void AddKeyPressedEventFunc(std::function<bool(const Event&)> func);
		void AddKeyReleasedEventFunc(std::function<bool(const Event&)> func);

		void AddMouseMoveEventFunc(std::function<bool(const Event&)> func);

	private:
		WindowData data;
		void* windowPtr = nullptr;

		void SetupEvents();

	};

}