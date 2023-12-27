#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace Copper {

	class Collider : public Component {

		friend class Registry;
		friend class RigidBody;

	public:
		enum Type : uint8_t {

			Box = 1,
			Sphere = 2,

		};

		bool trigger = false;

	protected:
		Type type = (Type) 0;

	};

}