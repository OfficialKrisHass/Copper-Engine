#include "cupch.h"
#include "Engine/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Copper {

	void RendererAPI::Initialize() {

		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) LogError("Failed to load GLAD!");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glPointSize(10.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	void RendererAPI::ClearColor(float r, float g, float b) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(r, g, b, 1.0f);

	}

	void RendererAPI::Render(VertexArray* vao) {

		vao->Bind();
		shader->Bind();

		glDrawElements(GL_TRIANGLES, vao->Count(), GL_UNSIGNED_INT, 0);

	}

}