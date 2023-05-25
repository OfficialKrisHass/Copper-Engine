#pragma once

#include "Engine/Events/Event.h"

namespace Copper {

	class WindowCloseEvent : public Event {


	};

	class WindowFocusedEvent : public Event {

	public:
		WindowFocusedEvent() = default;

		bool focused;

	};

	class WindowResizeEvent : public Event {

	public:
		WindowResizeEvent() = default;
		WindowResizeEvent(uint32_t width, uint32_t height) : width(width), height(height) {}

		uint32_t width;
		uint32_t height;

	};

}