#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Shader.h"

namespace Copper {

	class VertexArray;

	class Transform;
	class Camera;
	class Light; 
	class Mesh;

	namespace Renderer {

		void Initialize();
		
		void ClearColor(float r, float g, float b);
		void ResizeViewport(const UVector2I& size);

		void RenderFrame(Camera* cam, Light* light);
		void EndFrame();

		void AddMesh(Mesh* mesh, Transform* transform);

		void SetShaderPath(const std::string& vertexPath, const std::string& fragmentPath);
		Shader* GetShader();

	}

	class RendererAPI {

	public:
		static void Initialize();

		void ClearColor(float r, float g, float b);
		void ResizeViewport(const UVector2I& size);

		void Render(VertexArray* vao, uint32 count, Camera* cam, Light* light);
		void EndFrame();

		void SetShader(const std::string& vertexPath, const std::string& fragmentPath) { this->m_shader = Shader(vertexPath, fragmentPath); }
		Shader* GetShader() { return &this->m_shader; }
		
	private:
		Shader m_shader;

	};

}