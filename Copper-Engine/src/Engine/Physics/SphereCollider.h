#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Collider.h"

namespace Copper {

	class SphereCollider : public Collider {

	public:
		float radius = 1.0f;

		CollisionData Intersects(SphereCollider* other);

		ColliderType(Sphere)
	private:

	};

}