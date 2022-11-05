#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Mesh.h"

#include <CopperECS/Component.h>

namespace Copper {

	class MeshRenderer : public Component {

	public:
		MeshRenderer() = default;

		std::vector<Mesh> meshes;

	};

}