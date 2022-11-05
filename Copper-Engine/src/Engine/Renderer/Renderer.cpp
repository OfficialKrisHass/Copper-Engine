#include "cupch.h"
#include "Renderer.h"

#include <GLM/ext/matrix_transform.hpp>

namespace Copper::Renderer {

	std::vector<float> vertices{
		//Position				//Color				//Normals
		-0.5f, -0.5f,  0.0f,    1.0f, 0.0f, 0.0f,	0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.0f,    0.0f, 0.0f, 1.0f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 1.0f,	0.0f,  0.0f, -1.0f,

	};

	std::vector<uint32_t> indices{

		0, 1, 2,
		2, 3, 0,

	};

	struct RendererData {

		static const uint32_t maxVertices = 20000;

		Shared<VertexArray> vao;
		Shared<VertexBuffer> vbo;
		Shared<IndexBuffer> ibo;

		std::vector<float> vertices;
		std::vector<uint32_t> indices;
		
	};

	RendererData data;
	RendererAPI api;

	void Initialize() {

		data.vao = CreateShared<VertexArray>();
		
		data.vbo = CreateShared<VertexBuffer>(data.maxVertices * sizeof(float));
		data.vbo->SetLayout({ {"Position", ElementType::Float3}, {"Color", ElementType::Float3}, {"Normal", ElementType::Float3} });

		data.ibo = CreateShared<IndexBuffer>(data.maxVertices * 1.5f * sizeof(uint32_t));

		data.vao->SetVertexBuffer(data.vbo);
		data.vao->SetIndexBuffer(data.ibo);

		data.vbo->Unbind();
		data.vao->Unbind();

	}

	void Render(Camera* cam, Light* light) {

		data.vbo->SetData(data.vertices);
		data.ibo->SetData(data.indices);

		api.Render(data.vao, (uint32_t) data.indices.size(), cam, light);

		data.vertices.clear();
		data.indices.clear();
		
	}

	void AddMesh(Mesh* mesh, Transform* transform) {

		glm::mat4 mat = transform->CreateMatrix();
		glm::mat4 noScale(1.0f);

		noScale = glm::translate(noScale, (glm::vec3) transform->position);
		noScale = glm::rotate(noScale, glm::radians(transform->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		noScale = glm::rotate(noScale, glm::radians(transform->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		noScale = glm::rotate(noScale, glm::radians(transform->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		int numOfVertices = (int) data.vertices.size() / 9;

		for (int i = 0; i < mesh->vertices.size(); i++) {

			Vector3 position;
			Vector3 normal;
			Color color;

			position = mat * glm::vec4((glm::vec3) mesh->vertices[i], 1.0f);
			normal = (glm::mat3) noScale * (glm::vec3) mesh->normals[i];
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
	void SetShader(Shader* shader) { api.SetShader(shader); }
	Shader* GetShader() { return api.GetShader(); }


}