#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Collider.h"

namespace Copper {

	class SphereCollider : public Collider {

		friend Scene;

	public:
		float radius = 0.5f;
		Vector3 center = Vector3::zero;

	private:
		void Setup();

	};

}