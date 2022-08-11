#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"

namespace Copper {

	namespace Renderer {

		void Initialize();
		
		void ClearColor(float r, float g, float b);
		void Render(VertexArray* vao);

		void SetShader(Shader* shader);

	}

	class RendererAPI {

	public:
		static void Initialize();

		void ClearColor(float r, float g, float b);
		void Render(VertexArray* vao);

		inline void SetShader(Shader* shader) { this->shader = shader; }
		
	private:
		Shader* shader;

	};

}