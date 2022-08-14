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

	void Render(Mesh* mesh, Camera* cam) {

		api.Render(mesh, cam);

	}

	void SetShader(Shader* shader) {

		api.SetShader(shader);

	}

}