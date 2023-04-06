#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

#include "Engine/Physics/CollisionData.h"

#define ColliderType(x) Collider::Type GetColliderType() const override { return Collider::Type::x; }

namespace Copper {

	class Collider : public Component {

		friend class Scene;

	protected:
		enum class Type;

	public:
		CollisionData Intersects(Collider* other);

	private:
		bool checkedAllCollisions = false;

		virtual Type GetColliderType() const = 0;

	protected:
		enum class Type {

			Invalid,

			Sphere,
			Box

		};

	};

}