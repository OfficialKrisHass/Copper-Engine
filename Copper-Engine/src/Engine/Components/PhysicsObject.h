#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx { class PxRigidActor; }

namespace Copper {

	class PhysicsObject : public Component {

		friend class Scene;

	public:
		PhysicsObject() = default;
		~PhysicsObject() {}

		float mass = 1.0f;
		bool isStatic = false;

	private:
		physx::PxRigidActor* actor;

	};

}