#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"

namespace Copper {

	std::vector<float> vertices{

		-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 1.0f,

	};

	std::vector<uint32_t> indices{

		0, 1, 2,
		2, 3, 0

	};

	struct EngineData {

		bool running = true;

		Window* window;

		VertexBuffer* vbo;
		IndexBuffer* ibo;
		VertexArray* vao;

	};

	EngineData data;

	void Initialize() {

		Logger::Initialize();

		Log("--------------------Engine Initialization");
		Log("Engine Initialiation started");

		data.window = new Window(WindowData("Copper Engine", 800, 600));

		Renderer::SetShader(new Shader("assets/Shaders/vertexDefault.glsl", "assets/Shaders/fragmentDefault.glsl"));

		data.vao = new VertexArray();
		data.vbo = new VertexBuffer(vertices, { {"Position", ElementType::Float3}, {"Color", ElementType::Float3} });
		data.ibo = new IndexBuffer(indices);

		data.vao->SetVertexBuffer(data.vbo);
		data.vao->SetIndexBuffer(data.ibo);

		data.vbo->Unbind();
		data.vao->Unbind();

		Log("Engine Succesfully Initialized");
		Log("--------------------Engine Initialization\n");

	}

	void Run() {

		Log("--------------------Engine Run Loop");
		Log("Engine Entered the Run Loop");

		while (data.running) {

			Renderer::ClearColor(0.18f, 0.18f, 0.18f);
			Renderer::Render(data.vao);

			data.window->Update();

		}

		Log("Engine Succesfully Exited the Run Loop");
		Log("--------------------Engine Run Loop\n");

	}

	void Shutdown() {

		Log("--------------------Engine Shutdown");
		Log("Engine Entered the Shutdown");

		data.window->Shutdown();

		Log("Engine Succesfully Shutdown");
		Log("--------------------Engine Shutdown\n");

		//std::cin.get();

	}

	Window GetWindow() { return *data.window; }

	bool OnWindowResize(Event& e) {

		return true;

	}

	bool OnWindowClose(Event& e) {

		data.running = false;

		return true;

	}

}