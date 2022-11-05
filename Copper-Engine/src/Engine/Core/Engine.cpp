#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>

namespace Copper {

	struct EngineData {

		bool running = true;
		UVector2I windowSize;

		FrameBuffer* fbo;
		Window* window;

		Scene* scene;

		void (*EditorRun)();
		void (*EditorUI)();
			
	};

	EngineData data;

	void Initialize() {

		Logger::Initialize();

		Log("--------------------Engine Initialization");
		Log("Engine Initialiation started");

		data.windowSize = UVector2I(1280, 720);
		data.window = new Window(WindowData("Copper Engine", 1280, 720));
		data.fbo = new FrameBuffer(data.windowSize);

		Renderer::Initialize();
		Renderer::SetShader(new Shader("assets/Shaders/vertexDefault.glsl", "assets/Shaders/fragmentDefault.glsl"));
		UI::Initialize();

		Log("Engine Succesfully Initialized");
		Log("--------------------Engine Initialization\n");

	}

	void Run() {

		Log("--------------------Engine Run Loop");
		Log("Engine Entered the Run Loop");

		while (data.running) {

			if (data.fbo->Width() != data.windowSize.x || data.fbo->Height() != data.windowSize.y) {
			
				data.fbo->Resize(data.windowSize);
				data.scene->cam->Resize(data.windowSize);
		
			}

			data.fbo->Bind();
			data.scene->Update();
			data.fbo->Unbind();

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
	void LoadScene(Scene* scene) {

		data.scene = scene;
		
	}

	bool OnWindowResize(Event& e) {

		return true;

	}

	bool OnWindowClose(Event& e) {

		data.running = false;

		return true;

	}

	//Getters
	Window GetWindow() { return *data.window; }
	uint32_t GetFBOTexture() { return data.fbo->GetColorAttachment(); }

	Scene* GetScene() { return data.scene; }

	//Setters
	void SetWindowSize(UVector2I size) { data.windowSize = size; }

	void SetEditorRunFunc(void (*func)()) { data.EditorRun = func; }
	void SetEditorUIFunc(void (*func)()) { data.EditorUI = func; }

}
