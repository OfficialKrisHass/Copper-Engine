#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#define COLLIDER_TYPES 3

namespace physx { class PxShape; }

namespace Copper {

  class RigidBody;

	class Collider : public Component {

		friend class Registry;
		friend class Scene;

		friend RigidBody;

	public:
		bool trigger = false;
		Vector3 center = Vector3::zero;

		enum Type : uint8 {

			None,

			Box,
			Sphere,
			Capsule,

		};

		void Setup();

        inline Type GetType() const { return m_type; }
        inline RigidBody* GetRigidBody() const { return m_rb; }

	protected:
		Type m_type = Type::None;
		RigidBody* m_rb = nullptr;

	private:
		virtual physx::PxShape* CreateShape() const = 0;

	};

}
