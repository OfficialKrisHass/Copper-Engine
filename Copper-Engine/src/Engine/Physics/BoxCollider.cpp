#include "cupch.h"
#include "BoxCollider.h"

#include "Engine/Components/Transform.h"

namespace Copper {

	CollisionData BoxCollider::Intersects(BoxCollider* other) {

		Vector3 min = GetTransform()->position - size / 2.0f;
		Vector3 max = GetTransform()->position + size / 2.0f;
		Vector3 otherMin = other->GetTransform()->position - other->size / 2;
		Vector3 otherMax = other->GetTransform()->position + other->size / 2;

		Vector3 dist1 = min - otherMax;
		Vector3 dist2 = otherMin - max;

		Vector3 distance;
		distance.x = dist1.x > dist2.x ? dist1.x : dist2.x;
		distance.y = dist1.y > dist2.y ? dist1.y : dist2.y;
		distance.z = dist1.z > dist2.z ? dist1.z : dist2.z;

		/*bool ret = true;
		if (distance.x > 0.0f || distance.y > 0.0f || distance.z > 0.0f) ret = false;*/
		float maxDistance = distance.x;

		if (distance.y > maxDistance) maxDistance = distance.y;
		if (distance.z > maxDistance) maxDistance = distance.z;

		return CollisionData {distance, maxDistance >= 0.0f, this, other};

	}

}