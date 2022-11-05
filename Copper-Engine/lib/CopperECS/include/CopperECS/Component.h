#pragma once

#include "Engine/Components/Transform.h"

namespace Copper {

	class Object;

	struct Component {

	public:
		Object* object;
		Transform* transform;

	};

}