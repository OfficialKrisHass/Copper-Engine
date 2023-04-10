#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

#include "Engine/Physics/CollisionData.h"

#define ColliderType(x) Collider::Type GetColliderType() const override { return Collider::Type::x; }

namespace Copper {

	class Collider : public Component {

		friend class Scene;

	public:
		CollisionData Intersects(Collider* other);

		enum class Type;
		virtual Type GetColliderType() const = 0;

	private:
		bool checkedAllCollisions = false;

	public:
		enum class Type {

			Invalid,

			Sphere,
			Box

		};

	};

}