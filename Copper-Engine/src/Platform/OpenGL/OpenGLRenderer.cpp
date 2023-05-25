#include "cupch.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Core/Engine.h"

#include "Engine/Components/Transform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Copper {

#ifndef CU_EDITOR
	static const float gameRectVertices[20] {

		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f

	};
	static const uint32_t gameRectIndices[6] {

		0, 1, 2,
		2, 3, 0

	};
#endif

	uint32_t gameRectVAO;
	Shader gameRectShader;

	void RendererAPI::Initialize() {

		CHECK((GetEngineState() == EngineState::Initialization), "Cannot Initialize the Renderer API, current Engine State is: {}", EngineStateToString(GetEngineState()))

		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) LogError("Failed to load GLAD!");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glPointSize(10.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	#ifndef CU_EDITOR
		//
		//Setup the Game Rect
		//
		gameRectShader = Shader("assets/Shaders/gameRectVertex.glsl", "assets/Shaders/gameRectFragment.glsl");

		glGenVertexArrays(1, &gameRectVAO);
		glBindVertexArray(gameRectVAO);

		//VBO
		uint32_t vbo;
		glGenBuffers(1, &vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, gameRectVertices, GL_STATIC_DRAW);

		//IBO
		uint32_t ibo;
		glGenBuffers(1, &ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, gameRectIndices, GL_STATIC_DRAW);

		//Load the data into the VAO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (sizeof(float) * 3));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	#endif

	}

	void RendererAPI::ClearColor(float r, float g, float b) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(r, g, b, 1.0f);

	}
	void RendererAPI::ResizeViewport(const UVector2I& size) {

		glViewport(0, 0, size.x, size.y);

	}
	
	void RendererAPI::Render(VertexArray* vao, uint32_t count, Camera* cam, Light* light) {

		vao->Bind();
		shader.Bind();

		shader.LoadMat4TEST("ProjectionView", (glm::mat4) cam->CreateProjectionMatrix() * (glm::mat4) cam->CreateViewMatrix());
		shader.LoadVec3("camPos", cam->GetTransform()->position);

		if (light) {

			shader.LoadVec3("light.position", light->GetTransform()->GlobalPosition());
			shader.LoadVec3("light.color", light->color);
			shader.LoadFloat("light.intensity", light->intensity);

		}

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

	}
	void RendererAPI::EndFrame() {

	#ifndef CU_EDITOR
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.18f, 0.18f, 0.18f, 1.0f);

		gameRectShader.Bind();
		glBindVertexArray(gameRectVAO);

		glBindTexture(GL_TEXTURE_2D, GetFBOTexture());

		gameRectShader.LoadInt("tex", 0);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	#endif

	}

}