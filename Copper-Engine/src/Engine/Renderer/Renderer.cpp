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

	void Render(Mesh* mesh, Camera* cam, Light* light) {

		api.Render(mesh, cam, light);

	}

	void SetShader(Shader* shader) {

		api.SetShader(shader);

	}

	Shader* GetShader() {

		return api.GetShader();
		
	}


}