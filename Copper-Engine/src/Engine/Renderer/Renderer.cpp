#include "cupch.h"
#include "Renderer.h"

namespace Copper::Renderer {

	RendererAPI api;

	void Initialize() {

		RendererAPI::Initialize();

	}

	void ClearColor(float r, float g, float b) {

		api.ClearColor(r, g, b);

	}

	void Render(VertexArray* vao) {

		api.Render(vao);

	}

	void SetShader(Shader* shader) {

		api.SetShader(shader);

	}

}