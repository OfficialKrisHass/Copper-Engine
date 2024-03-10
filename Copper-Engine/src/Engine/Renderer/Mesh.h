#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Material.h"

namespace Copper {

	class Mesh {

	public:
		Mesh() = default;

		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Color> colors;
		std::vector<Vector2> uvs;

		std::vector<uint32> indices;

		MaterialAsset material;

	};

}