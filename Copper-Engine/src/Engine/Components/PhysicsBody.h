#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"
#include "Engine/Components/Transform.h"

#include <GLM/glm.hpp>

#include <GLM/gtx/euler_angles.hpp>

#define GravityConstant 9.81f

namespace Copper {

	class PhysicsBody : public Component {

		friend class Scene;

	public:
		glm::quat q; //TO DO: Temporary, remove this and put it into Transform

		float mass = 1.0f;

		Vector3 linearVelocity;
		Vector3 angularVelocity;
			
		bool staticBody = false;
		bool gravity = true;

	private:
		glm::mat3 IbodyInv = glm::mat3(1.0f);

		Vector3 force;
		Vector3 torque;

	};

}