#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

#define GravityConstant 9.81f

namespace Copper {

	class PhysicsBody : public Component {

	public:
		Vector3 velocity;

		bool staticBody = false;
		bool shouldGravity = true;
		/*float mass = 1.0f;
		float angle = 0.0f;
		float angularVelocity = 0.0f;
		Vector3 force;
		float torque;
		float momentOfIntertia = 1.0f * (1.0f + 1.0f) / 12.0f;*/

	private:

	};

}