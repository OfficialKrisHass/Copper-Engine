#pragma once

#include "Engine/Events/Event.h"

namespace Copper {

	class MouseMoveEvent : public Event {

	public:
		MouseMoveEvent() = default;
		MouseMoveEvent(Vector2 mouseCoords) : mouseCoords(mouseCoords) {}

		Vector2 mouseCoords;

	};

}