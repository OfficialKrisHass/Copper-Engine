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
		
		void ClearColor(const Color& color);
		void ResizeViewport(const UVector2I& size);

		void StartBatch();
		void RenderBatch();
		void NewBatch();

		void AddMesh(Mesh* mesh, Transform* transform);

		void SetCamera(Camera* cam);
		void SetLight(Light* light);

		void SetWireframe(bool value = true);
		void SetShaderPath(const std::string& vertexPath, const std::string& fragmentPath);

		bool GetWireframe();

	}

	// A very low Level wrapper for the renderer api
	// Source is in the Platform/<API> for the current API
	namespace RendererAPI {

		void Initialize();

		void ClearColor(const Color& color);
		void ResizeViewport(const UVector2I& size);

		void Render(VertexArray* vao, uint32 count);
		void EndFrame();

		void SetCamera(Camera* cam);
		void SetLight(Light* light);

		void SetWireframe(bool value);
		void SetShaderPath(const std::string& vertexPath, const std::string& fragmentPath);

	}

}