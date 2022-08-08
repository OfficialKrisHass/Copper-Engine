#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/WindowEvent.h"

#include <GLFW/glfw3.h>

namespace Copper {

	struct WindowData {

		uint32_t width;
		uint32_t height;
		std::string title;

		WindowResizeEvent wResE;
		WindowCloseEvent wClsE;

		WindowData() = default;
		WindowData(std::string title, uint32_t width, uint32_t height) : title(title), width(width), height(height) {}

	};

	class Window {

	public:
		Window(WindowData data);

		void Update();
		void Shutdown();

		void* windowPtr;

	private:
		WindowData data;

	};

}