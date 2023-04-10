#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Collider.h"

namespace Copper {

	class BoxCollider : public Collider {

	public:
		Vector3 size = Vector3::one;

		CollisionData Intersects(BoxCollider* other);

		ColliderType(Box)
	private:

	};

}