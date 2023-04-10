#include "cupch.h"
#include "Collider.h"

#include "Engine/Components/Transform.h"

#include "Engine/Physics/SphereCollider.h"
#include "Engine/Physics/BoxCollider.h"

#define CompareColliderTypes(x, y) GetColliderType() == Type::x && other->GetColliderType() == Type::y

namespace Copper {

	CollisionData Collider::Intersects(Collider* other) {

		if(CompareColliderTypes(Sphere, Sphere))
			return ((SphereCollider*) this)->Intersects((SphereCollider*) other);

		if(CompareColliderTypes(Box, Box))
			return ((BoxCollider*) this)->Intersects((BoxCollider*) other);

	}

	/*IntersectData Collider::Intersects(Collider* other) {

		Vector3 min = GetTransform()->position - size / 2;
		Vector3 max = GetTransform()->position + size / 2;
		Vector3 otherMin = other->GetTransform()->position - other->size / 2;
		Vector3 otherMax = other->GetTransform()->position + other->size / 2;

		float d1x = min.x - otherMax.x;
		float d2x = otherMin.x - max.x;
		float d1y = min.y - otherMax.y;
		float d2y = otherMin.y - max.y;

		bool ret = true;

		if (d1x > 0.0f || d1y > 0.0f) ret = false;
		if (d2x > 0.0f || d2y > 0.0f) ret = false;

		return IntersectData {min, ret};

	}

	IntersectData Collider::Intersects(const Vector3& min, const Vector3& max) {

		Vector3 thisMin = GetTransform()->position - size / 2;
		Vector3 thisMax = GetTransform()->position + size / 2;

		bool ret = thisMin.x <= max.x && thisMax.x >= min.x &&
			       thisMin.y <= max.y && thisMax.y >= min.y &&
			       thisMin.z <= max.z && thisMax.z >= min.z;

		return IntersectData {thisMin, ret};;

	}*/

}