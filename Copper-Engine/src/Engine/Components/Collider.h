#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

#include "Engine/Physics/IntersectData.h"

namespace Copper {

	class Collider : public Component {

		friend class Scene;

	public:
		IntersectData Intersects(Collider* other);
		IntersectData Intersects(const Vector3& min, const Vector3& max);

		Vector3 size = Vector3::zero;
		
	private:
		bool checkedAllColliders = false;

	};

}