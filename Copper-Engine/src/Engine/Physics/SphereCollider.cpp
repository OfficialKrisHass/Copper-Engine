#include "cupch.h"
#include "SphereCollider.h"

#include "Engine/Components/Transform.h"

namespace Copper {

	CollisionData SphereCollider::Intersects(SphereCollider* other) {

		Vector3 direction = other->GetTransform()->position - GetTransform()->position;

		float radiusDistance = radius + other->radius;
		float centreDistance = direction.Length();
		float distance = centreDistance - radiusDistance;

		direction /= centreDistance;

		return CollisionData {direction * distance, distance >= 0.0f, this, other};

	}

}