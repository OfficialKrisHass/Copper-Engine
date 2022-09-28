#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Material.h"

#include "Engine/Scene/Components/Component.h"

#include "cupch.h"

namespace Copper {

	class Mesh : public Component {

	public:
		Mesh() = default;

		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		Material material;

		void Regenerate();
		void Test();
		void GetMat();

		inline VertexArray* Vao() { return vao; }
		void Update();

	private:
		VertexArray* vao;

	};

}