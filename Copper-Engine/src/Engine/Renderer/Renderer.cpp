#include "cupch.h"
#include "Renderer.h"

#include "Engine/Core/Engine.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Components/Transform.h"
#include "Engine/Components/Light.h"

#include <GLM/ext/matrix_transform.hpp>

namespace Copper::Renderer {

	constexpr uint32 MaxVertices = 20'000;
	constexpr uint32 MaxIndices = (uint32) (MaxVertices * 1.5f);

	constexpr uint32 MaxLines = 10'000;
	constexpr uint32 MaxLineVertices = MaxLines * 2;

	struct Vertex {

		Vector3 position;
		Vector3 color;
		Vector3 normal;

		Vector2 uv;
		float textureIndex;

	};
	struct LineVertex {

		Vector3 pos;
		Vector3 color;

	};

	struct RendererData {

		// 3D Geometry

		VertexArray vao;
		VertexBuffer vbo;
		IndexBuffer ibo;

		uint32 verticesCount = 0;
		uint32 indicesCount = 0;

		Vertex* vertices = new Vertex[MaxVertices];
		uint32* indices = new uint32[MaxIndices];

		// Lights

		Light** lights = new Light*[MAX_LIGHTS];
		uint32 lightCount = 0;

		// Lines

		VertexArray lineVao;
		VertexBuffer lineVbo;

		uint32 lineVertexCount = 0;
		LineVertex* lineVertices = new LineVertex[MaxLineVertices];

		// Misc.

		uint32 drawCalls = 0;

		Color skyboxColor = Color(0.18f, 0.18f, 0.18f);

		bool wireframe = false;

		// Tmp.
		Texture wallTexture;
		Texture gridboxTexture;
		
	};

	RendererData data;

	void Initialize() {

		VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the Renderer");

		RendererAPI::Initialize();

		// 3D Geometry

		data.vao = VertexArray(nullptr);
		
		data.vbo = VertexBuffer(nullptr, MaxVertices * sizeof(float), {

			ElementType::Vec3, // Position
			ElementType::Vec3, // Color
			ElementType::Vec3, // Normal

			ElementType::Vec2, // UV
			ElementType::Float, // Texture Index

		});
		data.ibo = IndexBuffer(nullptr, MaxIndices * sizeof(uint32));

		data.vao.SetVertexBuffer(&data.vbo);
		data.vao.SetIndexBuffer(&data.ibo);
				
		data.vbo.Unbind();
		data.vao.Unbind();

		// Lines

		data.lineVao = VertexArray(nullptr);

		data.lineVbo = VertexBuffer(nullptr, MaxLineVertices * sizeof(LineVertex), {

			ElementType::Vec3, // Position
			ElementType::Vec3, // Color

		});
		data.lineVao.SetVertexBuffer(&data.lineVbo);

		data.lineVbo.Unbind();
		data.lineVao.Unbind();

		data.wallTexture.Create("assets/Textures/wall.png");
		data.gridboxTexture.Create("assets/Textures/gridbox.png");

	}

	void StartFrame() {

		data.drawCalls = 0;
		data.lightCount = 0;

		data.lineVertexCount = 0;

		RendererAPI::ClearColor(data.skyboxColor);

		StartBatch();

	}
	void EndFrame() {

		RendererAPI::EndFrame();

	}

	void StartBatch() {

		data.verticesCount = 0;
		data.indicesCount = 0;

	}
	void RenderBatch() {

		if (data.verticesCount == 0 || data.indicesCount == 0) return;

		data.vbo.SetData((float*) data.vertices, data.verticesCount * 12);
		data.ibo.SetData(data.indices, data.indicesCount);

		RendererAPI::Render(&data.vao, data.indicesCount, data.lights, data.lightCount, &data.wallTexture, &data.gridboxTexture);

		data.drawCalls++;

	}
	void NewBatch() {

		RenderBatch();
		StartBatch();

	}

	void RenderLines() {

		if (data.lineVertexCount == 0) return;

		data.lineVbo.SetData((float*) data.lineVertices, data.lineVertexCount * 6);
		RendererAPI::RenderLines(&data.lineVao, data.lineVertexCount);

		data.drawCalls++;

	}

	void AddMesh(Mesh* mesh, Transform* transform) {

		const Matrix4& transformMat = transform->TransformMatrix();

		const uint32 indicesCount = (uint32) mesh->indices.size();
		const uint32 verticesCount = (uint32) mesh->vertices.size();

		if (data.indicesCount + indicesCount > MaxIndices ||
			data.verticesCount + verticesCount > MaxVertices)
			NewBatch();

		uint32 texIndex = transform->GetEntity()->name == "Ground" ? 1 : 0;
		for (uint32 i = 0; i < verticesCount; i++) {

			Vertex& vertex = data.vertices[data.verticesCount + i];

			vertex.position = transformMat * Vector4(mesh->vertices[i], 1.0f);
			vertex.normal = (Matrix3) transformMat * mesh->normals[i];
			vertex.color = mesh->colors[i];
			vertex.uv = mesh->uvs[i];
			vertex.textureIndex = texIndex;

		}
		for (uint32 i = 0; i < indicesCount; i++)
			data.indices[data.indicesCount + i] = mesh->indices[i] + data.verticesCount;

		data.verticesCount += verticesCount;
		data.indicesCount += indicesCount;

	}

	void AddLine(const Vector3& start, const Vector3& end, const Color& color) {

		if (data.lineVertexCount >= MaxLineVertices) {

			LogError("Max amount of lines reached ({})", MaxLines);
			return;

		}

		LineVertex* vertex = &data.lineVertices[data.lineVertexCount];
		vertex->pos = start;
		vertex->color = color;

		vertex = &data.lineVertices[data.lineVertexCount + 1];
		vertex->pos = end;
		vertex->color = color;

		data.lineVertexCount += 2;

	}
	void AddLine(const Vector3& start, const Vector3& end, const Color& color, Transform* transform) {

		AddLine(transform->TransformMatrix() * Vector4(start, 1.0f), transform->TransformMatrix() * Vector4(end, 1.0f), color);

	}
	void AddCube(const Vector3& centre, const Vector3& size, const Color& color, Transform* transform) {

		const Vector3 half = size / 2.0f;

		// Bottom

		static const Vector3 v0 = Vector3(-half.x, -half.y, half.z);
		static const Vector3 v1 = Vector3(half.x, -half.y, half.z);
		static const Vector3 v2 = Vector3(half.x, -half.y, -half.z);
		static const Vector3 v3 = Vector3(-half.x, -half.y, -half.z);

		// Top

		static const Vector3 v4 = Vector3(-half.x, half.y, half.z);
		static const Vector3 v5 = Vector3(half.x, half.y, half.z);
		static const Vector3 v6 = Vector3(half.x, half.y, -half.z);
		static const Vector3 v7 = Vector3(-half.x, half.y, -half.z);

		// Lines

		AddLine(centre + v0, centre + v1, color, transform);
		AddLine(centre + v1, centre + v2, color, transform);
		AddLine(centre + v2, centre + v3, color, transform);
		AddLine(centre + v3, centre + v0, color, transform);

		AddLine(centre + v4, centre + v5, color, transform);
		AddLine(centre + v5, centre + v6, color, transform);
		AddLine(centre + v6, centre + v7, color, transform);
		AddLine(centre + v7, centre + v4, color, transform);

		AddLine(centre + v0, centre + v4, color, transform);
		AddLine(centre + v1, centre + v5, color, transform);
		AddLine(centre + v2, centre + v6, color, transform);
		AddLine(centre + v3, centre + v7, color, transform);

	}

	void AddLight(Light* light) {

		CU_ASSERT(data.lightCount < MAX_LIGHTS, "Can't add another light, reached maximum amount of lights allowed ({})", MAX_LIGHTS);

		data.lights[data.lightCount] = light;
		data.lightCount++;

	}
	void ClearLights() {

		data.lightCount = 0;

	}

	void Render(Camera* cam, bool gizmos) {

		RendererAPI::SetCamera(cam);
		RendererAPI::Render(&data.vao, data.indicesCount, data.lights, data.lightCount, &data.wallTexture, &data.gridboxTexture);
		if (gizmos)
			Renderer::RenderLines();

	}

	void SetCamera(Camera* cam) { RendererAPI::SetCamera(cam); }

	void SetWireframe(bool value) {
		
		data.wireframe = value;
		RendererAPI::SetWireframe(value);
	
	}
	void SetShaderPath(const std::string& vertexPath, const std::string& fragmentPath) {
		
		VERIFY_STATE(EngineCore::EngineState::Initialization, "Set the Shader");
		RendererAPI::SetShaderPath(vertexPath, fragmentPath);
		
	}

	bool GetWireframe() { return data.wireframe; }

	Color& AmbientColor() { return RendererAPI::AmbientColor(); }
	Vector3& AmbientDirection() { return RendererAPI::AmbientDirection(); }

	Color& SkyboxColor() { return data.skyboxColor; }


}