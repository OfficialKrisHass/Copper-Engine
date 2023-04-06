#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	struct CollisionData {

		Vector3 distance;
		bool intersect;

		class Collider* a;
		class Collider* b;

		operator bool() const { return intersect; }

	};

}