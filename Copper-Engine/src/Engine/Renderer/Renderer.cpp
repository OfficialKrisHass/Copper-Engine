#include "cupch.h"
#include "Renderer.h"

#include "Engine/Core/Engine.h"

#include "Engine/Components/Transform.h"

#include <GLM/ext/matrix_transform.hpp>

namespace Copper::Renderer {

	struct RendererData {

		static const uint32_t maxVertices = 20000;

		VertexArray vao;
		VertexBuffer vbo;
		IndexBuffer ibo;

		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		
	};

	RendererData data;
	RendererAPI api;

	void Initialize() {

		CHECK((GetEngineState() == EngineState::Initialization), "Cannot Initialize the Renderer, current Engine State is: {}", EngineStateToString(GetEngineState()))

		data.vao = VertexArray(nullptr);
		
		data.vbo = VertexBuffer(data.maxVertices * sizeof(float));
		data.vbo.SetLayout({ {"Position", ElementType::Float3}, {"Color", ElementType::Float3}, {"Normal", ElementType::Float3} });

		data.ibo = IndexBuffer((uint32_t) (data.maxVertices * 1.5f * sizeof(uint32_t)));

		data.vao.SetVertexBuffer(&data.vbo);
		data.vao.SetIndexBuffer(&data.ibo);
				
		data.vbo.Unbind();
		data.vao.Unbind();

	}

	void RenderFrame(Camera* cam, Light* light) {

		data.vbo.SetData(data.vertices);
		data.ibo.SetData(data.indices);

		api.Render(&data.vao, (uint32_t) data.indices.size(), cam, light);
		
		
	}
	void EndFrame() {

		data.vertices.clear();
		data.indices.clear();

		api.EndFrame();

	}

	void AddMesh(Mesh* mesh, Transform* transform) {

		Matrix4 mat = transform->CreateMatrix();
		Matrix4 noScale = mat;
		CMath::ScaleMatrix(noScale, -transform->scale);

		int numOfVertices = (int) data.vertices.size() / 9;

		for (int i = 0; i < mesh->vertices.size(); i++) {

			Vector3 position;
			Vector3 normal;
			Color color;

			position = mat * Vector4(mesh->vertices[i], 1.0f);
			normal = (Matrix3) mat * mesh->normals[i];
			color = mesh->colors[i];

			data.vertices.push_back(position.x); data.vertices.push_back(position.y); data.vertices.push_back(position.z);
			data.vertices.push_back(color.r); data.vertices.push_back(color.g); data.vertices.push_back(color.b);
			data.vertices.push_back(normal.x); data.vertices.push_back(normal.y); data.vertices.push_back(normal.z);

		}

		for (int i = 0; i < mesh->indices.size(); i++) {

			data.indices.push_back(mesh->indices[i] + numOfVertices);

		}

	}

	void ClearColor(float r, float g, float b) { api.ClearColor(r, g, b); }
	void ResizeViewport(const UVector2I& size) { api.ResizeViewport(size); }

	void SetShader(const Shader& shader) { api.SetShader(shader); }
	Shader* GetShader() { return api.GetShader(); }


}