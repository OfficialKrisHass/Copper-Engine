#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	struct IntersectData {

		Vector3 distance;
		bool intersect;

		operator bool() const { return intersect; }

	};

}