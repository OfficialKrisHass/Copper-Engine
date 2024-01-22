#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Mesh.h"

#include "Engine/Components/Component.h"

namespace Copper {

	class MeshRenderer : public Component {

	public:
		MeshRenderer() = default;

		std::vector<Mesh> meshes;

	};

}