#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace Copper {

	class Collider : public Component {

	public:
		enum Type : uint8_t {

			Box = 1,

		};

		Type type;

		bool trigger = false;

	};

}