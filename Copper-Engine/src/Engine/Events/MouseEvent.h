#pragma once

#include "Engine/Events/Event.h"

namespace Copper {

	class MouseMoveEvent : public Event {

	public:
		MouseMoveEvent() = default;
		MouseMoveEvent(UVector2I mouseCoords) : mouseCoords(mouseCoords) {}

		UVector2I mouseCoords;

	};

}