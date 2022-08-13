#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Scene/Components/Mesh.h"

namespace Copper {

	namespace Renderer {

		void Initialize();
		
		void ClearColor(float r, float g, float b);
		void Render(Mesh* mesh);

		void SetShader(Shader* shader);

	}

	class RendererAPI {

	public:
		static void Initialize();

		void ClearColor(float r, float g, float b);
		void Render(Mesh* mesh);

		inline void SetShader(Shader* shader) { this->shader = shader; }
		
	private:
		Shader* shader;

	};

}