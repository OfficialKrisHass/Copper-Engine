#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class Registry;

	struct Tag {

		friend class Registry;

	public:
		std::string name = "Object";
		uint32_t index = 0;

	private:
		const static bool multipleOnOneObject = false;
		bool valid;

	};

}