#include "cupch.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Core/Engine.h"

#include "Engine/Renderer/VertexArray.h"

#include "Engine/Components/Transform.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"

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
	static const uint32 gameRectIndices[6] {

		0, 1, 2,
		2, 3, 0

	};
#endif

	uint32 gameRectVAO;
	Shader gameRectShader;

	void RendererAPI::Initialize() {

		VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the OpenGL Renderer API");

		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {

			LogError("Failed to load GLAD!");

		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glPointSize(10.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Copper Engine deafultly renders to the main FBO, meaning every app has to take the texture there and render it to a rect taking up the entire screen
	// TODO: Maybe we dont have to have or own shaders for this ? since it is just the basic shader, no lightning or MVPs ?
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
		uint32 ibo;
		glGenBuffers(1, &ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * 6, gameRectIndices, GL_STATIC_DRAW);

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
	
	void RendererAPI::Render(VertexArray* vao, uint32 count, Camera* cam, Light* light) {

		vao->Bind();
		m_shader.Bind();

		// TODO: Fix the CMath implementation of LoadMat4 not working
		m_shader.LoadMat4("ProjectionView", cam->CreateProjectionMatrix() * cam->CreateViewMatrix());
		m_shader.LoadVec3("camPos", cam->GetTransform()->Position());

		if (light) {

			m_shader.LoadVec3("light.position", light->GetTransform()->GlobalPosition());
			m_shader.LoadVec3("light.color", light->color);
			m_shader.LoadFloat("light.intensity", light->intensity);

		}

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

	}
	void RendererAPI::EndFrame() {
	
	// Take what was rendered to the main FBO and render it onto the gameRect
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