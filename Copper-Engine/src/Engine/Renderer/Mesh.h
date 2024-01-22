#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class Mesh {

	public:
		Mesh() = default;

		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Color> colors;
		std::vector<uint32> indices;

	};

}