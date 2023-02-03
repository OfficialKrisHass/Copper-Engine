#pragma once

#include "Engine/Events/Event.h"

#include "Engine/Input/KeyCodes.h"

namespace Copper {

	class KeyEvent : public Event {

	public:
		KeyEvent() = default;
		KeyEvent(KeyCode key) : key(key) {}

		KeyCode key;

	};

}