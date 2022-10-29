#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Scene/Components/Camera.h"
#include "Engine/Scene/Components/Light.h"

namespace Copper {

	namespace Renderer {

		void Initialize();
		
		void ClearColor(float r, float g, float b);
		void Render(Camera* cam, Light* light);

		void AddMesh(Mesh* mesh, Transform* transform);

		void SetShader(Shader* shader);
		Shader* GetShader();

	}

	class RendererAPI {

	public:
		static void Initialize();

		void ClearColor(float r, float g, float b);
		void Render(Shared<VertexArray> vao, uint32_t count, Camera* cam, Light* light);

		void SetShader(Shader* shader) { this->shader = shader; }
		Shader* GetShader() { return this->shader; }
		
	private:
		Shader* shader;

	};

}