#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx {

	class PxRigidDynamic;

}

namespace Copper {

	class PhysicsBody : public Component {

		friend class Scene;

	public:
		PhysicsBody() = default;
		~PhysicsBody() {}

		float mass = 1.0f;

	private:
		physx::PxRigidDynamic* body;

	};

}