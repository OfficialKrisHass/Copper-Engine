#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"
#include "Engine/Components/Transform.h"

#include <GLM/glm.hpp>

#include <GLM/gtx/euler_angles.hpp>

#define GravityConstant 9.81f

namespace Copper {

	class PhysicsBody : public Component {

	public:
		float mass = 1.0f;
		float inertia = 1.0f;

		glm::mat3 R = glm::mat3(1.0f);
		glm::quat q;

		Vector3 linearVelocity;
		Vector3 angularVelocity;
		glm::mat3 Iinv = glm::mat3(1.0f);
		glm::mat3 IbodyInv = glm::mat3(1.0f);

		Vector3 force;
		Vector3 torque;
			
		bool staticBody = false;
		bool shouldGravity = true;
		bool test = true;

		void UpdateR() {

			q = glm::quat(GetTransform()->rotation);

		}

	private:

	};

}