#pragma once

#include "Engine/Events/Event.h"

namespace Copper {

	class WindowCloseEvent : public Event {


	};

	class WindowFocusedEvent : public Event {

	public:
		WindowFocusedEvent() = default;

		bool focused = false;

	};

	class WindowResizeEvent : public Event {

	public:
		WindowResizeEvent() = default;
		WindowResizeEvent(uint32 width, uint32 height) : width(width), height(height) {}

		uint32 width = 0;
		uint32 height = 0;

	};

}