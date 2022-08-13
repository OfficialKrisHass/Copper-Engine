#pragma once

#include "Engine/Core/Core.h"

#include <string>

namespace Copper {

	struct Name {

		Name() = default;
		Name(std::string name) : name(name) {}

		std::string name;

	};

}