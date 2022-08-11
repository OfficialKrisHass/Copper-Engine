#pragma once

#include "Engine/Events/Event.h"

namespace Copper {

	class WindowCloseEvent : public Event {

	public:
		virtual std::string ToString() const override { return "Window Close Event"; }

	};

	class WindowResizeEvent : public Event {

	public:
		WindowResizeEvent() = default;
		WindowResizeEvent(uint32_t width, uint32_t height) : width(width), height(height) {}

		uint32_t width;
		uint32_t height;

		virtual std::string ToString() const override {

			std::stringstream ss;
			ss << "Window Resize Event: " << width << ", " << height;
			return ss.str();

		}

	};

}