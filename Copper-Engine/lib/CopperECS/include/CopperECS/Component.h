#pragma once

#include "Engine/Components/Transform.h"

namespace Copper {

	class Object;
	class Registry;

	struct Component {

		friend class Registry;

	public:
		Object* object;
		Transform* transform;

		uint32_t index = 0;

		bool Valid() const { return valid; }

	private:
		const static bool multipleOnOneObject = true;

		bool valid;

	};

}