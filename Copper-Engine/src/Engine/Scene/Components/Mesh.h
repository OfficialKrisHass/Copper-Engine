#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"

#include "Engine/Scene/Components/Component.h"

#include "cupch.h"

namespace Copper {

	struct Mesh : public Component {

	public:
		Mesh() = default;

		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		void Regenerate();

		inline VertexArray* Vao() { return vao; }

	private:
		VertexArray* vao;

	};

}