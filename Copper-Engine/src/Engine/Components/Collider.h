#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#define COLLIDER_TYPES 3

namespace physx { class PxShape; }

namespace Copper {

	class Collider : public Component {

		friend class Registry;
		friend class Scene;

		friend class RigidBody;

	public:
		enum Type;

		bool trigger = false;
		Vector3 center = Vector3::zero;

		enum Type : uint8 {

			None,

			Box,
			Sphere,
			Capsule,

		};

	protected:
		Type m_type = Type::None;
		RigidBody* m_rb = nullptr;

	private:
		void Setup();

		virtual physx::PxShape* CreateShape() const = 0;

	};

}