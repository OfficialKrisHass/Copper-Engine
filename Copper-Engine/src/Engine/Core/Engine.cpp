#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/UI/ImGui.h"

#include <GLM/vec3.hpp>
#include <ImGui/imgui.h>

namespace Copper {

	struct EngineData {

		bool running = true;

		Window* window;

		void (*EditorRun)();
		void (*EditorUI)();
			
	};

	EngineData data;

	void Initialize() {

		Logger::Initialize();

		Log("--------------------Engine Initialization");
		Log("Engine Initialiation started");

		data.window = new Window(WindowData("Copper Engine", 1280, 720));

		Renderer::SetShader(new Shader("assets/Shaders/vertexDefault.glsl", "assets/Shaders/fragmentDefault.glsl"));
		UI::Initialize();

		Log("Engine Succesfully Initialized");
		Log("--------------------Engine Initialization\n");

	}

	void Run() {

		Log("--------------------Engine Run Loop");
		Log("Engine Entered the Run Loop");

		while (data.running) {

			data.EditorRun();

			UI::Begin();
			data.EditorUI();
			UI::End();

			data.window->Update();

		}

		Log("Engine Succesfully Exited the Run Loop");
		Log("--------------------Engine Run Loop\n");

	}

	void Shutdown() {

		Log("--------------------Engine Shutdown");
		Log("Engine Entered the Shutdown");

		UI::Shutdown();
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

	void SetEditorRunFunc(void (*func)()) { data.EditorRun = func; }
	void SetEditorUIFunc(void (*func)()) { data.EditorUI = func; }

}