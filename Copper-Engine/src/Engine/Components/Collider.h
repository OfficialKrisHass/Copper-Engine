#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx { class PxShape; }

namespace Copper {

	class Collider : public Component {

		friend class Registry;
		friend class Scene;

		friend class RigidBody;

	public:
		enum Type : uint8_t {

			Box = 1,
			Sphere = 2,

		};

		bool trigger = false;

	protected:
		Type type = (Type) 0;

	private:
		void Setup();

		virtual physx::PxShape* CreateShape() const = 0;

	};

}