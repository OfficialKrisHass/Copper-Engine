#include "cupch.h"
#include "Renderer.h"

#include "Engine/Core/Engine.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Components/Transform.h"

#include <GLM/ext/matrix_transform.hpp>

namespace Copper::Renderer {

	constexpr uint32 MaxVertices = 20'000;
	constexpr uint32 MaxIndices = (uint32) (MaxVertices * 1.5f);

	struct Vertex {

		Vector3 position;
		Vector3 color;
		Vector3 normal;

	};

	struct RendererData {

		uint32 verticesCount = 0;
		uint32 indicesCount = 0;
		uint32 drawCalls = 0;

		VertexArray vao;
		VertexBuffer vbo;
		IndexBuffer ibo;

		Vertex vertices[MaxVertices];
		uint32 indices[MaxIndices];

		bool wireframe = false;
		
	};

	RendererData data;

	void Initialize() {

		VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the Renderer");

		RendererAPI::Initialize();

		data.vao = VertexArray(nullptr);
		
		data.vbo = VertexBuffer(MaxVertices * sizeof(float));
		data.vbo.SetLayout({ {"Position", ElementType::Float3}, {"Color", ElementType::Float3}, {"Normal", ElementType::Float3} });

		data.ibo = IndexBuffer(MaxIndices * sizeof(uint32));

		data.vao.SetVertexBuffer(&data.vbo);
		data.vao.SetIndexBuffer(&data.ibo);
				
		data.vbo.Unbind();
		data.vao.Unbind();

	}

	void StartFrame() {

		data.drawCalls = 0;

		StartBatch();

	}
	void RenderFrame() {

		RenderBatch();

	}
	void EndFrame() {

		RendererAPI::EndFrame();

		Log("Draw Calls: {}", data.drawCalls);

	}

	void Render(Camera* cam) {

		RendererAPI::SetCamera(cam);
		RendererAPI::Render(&data.vao, data.indicesCount);

	}

	void StartBatch() {

		data.verticesCount = 0;
		data.indicesCount = 0;

	}
	void RenderBatch() {

		data.vbo.SetData((float*) data.vertices, data.verticesCount * 9);
		data.ibo.SetData(data.indices, data.indicesCount);

		RendererAPI::Render(&data.vao, data.indicesCount);

		data.drawCalls++;

	}
	void NewBatch() {

		RenderBatch();
		StartBatch();

	}

	void AddMesh(Mesh* mesh, Transform* transform) {

		const Matrix4& transformMat = transform->TransformMatrix();

		const uint32 indicesCount = (uint32) mesh->indices.size();
		const uint32 verticesCount = (uint32) mesh->vertices.size();

		if (data.indicesCount + indicesCount > MaxIndices ||
			data.verticesCount + verticesCount > MaxVertices)
			NewBatch();

		for (uint32 i = 0; i < verticesCount; i++) {

			Vertex& vertex = data.vertices[data.verticesCount + i];

			vertex.position = transformMat * Vector4(mesh->vertices[i], 1.0f);
			vertex.normal = (Matrix3) transformMat * mesh->normals[i];
			vertex.color = mesh->colors[i];

		}
		for (uint32 i = 0; i < indicesCount; i++)
			data.indices[data.indicesCount + i] = mesh->indices[i] + data.verticesCount;

		data.verticesCount += verticesCount;
		data.indicesCount += indicesCount;

	}

	void ClearColor(const Color& color) { RendererAPI::ClearColor(color); }
	void ResizeViewport(const UVector2I& size) { RendererAPI::ResizeViewport(size); }

	void SetCamera(Camera* cam) { RendererAPI::SetCamera(cam); }
	void SetLight(Light* light) { RendererAPI::SetLight(light); }

	void SetWireframe(bool value) {
		
		data.wireframe = value;
		RendererAPI::SetWireframe(value);
	
	}
	void SetShaderPath(const std::string& vertexPath, const std::string& fragmentPath) {
		
		VERIFY_STATE(EngineCore::EngineState::Initialization, "Set the Shader");
		RendererAPI::SetShaderPath(vertexPath, fragmentPath);
		
	}

	bool GetWireframe() { return data.wireframe; }


}