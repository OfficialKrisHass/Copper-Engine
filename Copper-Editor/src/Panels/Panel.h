#pragma once

#include "Copper.h"

namespace Editor {

	class Panel {

	public:
		Panel() = default;
		Panel(std::string name) : name(name) {}

		std::string name;

		virtual void UIRender() = 0;

	};

}