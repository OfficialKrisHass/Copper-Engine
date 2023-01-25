#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Events/WindowEvent.h"
#include "Engine/Events/KeyEvent.h"

namespace Copper {

	struct WindowData {

		uint32_t width;
		uint32_t height;
		std::string title;

		WindowCloseEvent wClsE;
		WindowFocusedEvent wFocE;
		WindowResizeEvent wResE;

		KeyPresedEvent kPrsE;

		WindowData() = default;
		WindowData(std::string title, uint32_t width, uint32_t height) : title(title), width(width), height(height) {}

	};

	class Window {

	public:
		Window(WindowData data);

		void Update();
		void Shutdown();

		inline uint32_t Width() { return data.width; }
		inline uint32_t Height() { return data.height; }

		inline void* GetWindowPtr() { return windowPtr; }

	private:
		WindowData data;
		void* windowPtr;

	};

}