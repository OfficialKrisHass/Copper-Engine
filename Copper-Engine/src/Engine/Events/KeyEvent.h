#pragma once

#include "Engine/Events/Event.h"

#include "Engine/Input/KeyCodes.h"

namespace Copper {

	class KeyEvent : public Event {

	public:
		KeyEvent() = default;
		KeyEvent(KeyCode key) : key(key) {}

		virtual std::string ToString() const override {

			std::stringstream ss;
			ss << "Key Pressed Event: " << (uint16_t) key;
			return ss.str();

		}

		KeyCode key;

	};

}