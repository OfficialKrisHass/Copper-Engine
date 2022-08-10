#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"

namespace Copper {

	namespace Renderer {

		void COPPER_API Initialize();
		
		void COPPER_API ClearColor(float r, float g, float b);
		void COPPER_API Render(VertexArray* vao);

		void COPPER_API SetShader(Shader* shader);

	}

	class COPPER_API RendererAPI {

	public:
		static void Initialize();

		void ClearColor(float r, float g, float b);
		void Render(VertexArray* vao);

		inline void SetShader(Shader* shader) { this->shader = shader; }
		
	private:
		Shader* shader;

	};

}