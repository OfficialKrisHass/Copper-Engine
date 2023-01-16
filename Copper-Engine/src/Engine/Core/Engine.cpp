#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>

namespace Copper {

	struct EngineData {

		bool running = true;
		UVector2I windowSize;

		FrameBuffer* fbo;
		Window* window;

		Scene* scene;
		bool renderScene = false;

		void (*EditorRun)();
		void (*EditorUI)();

		bool (*EditorOnKeyPressed)(const Event&);
		bool (*EditorOnWindowClose)(const Event&);
			
	};

	EngineData data;

	void Initialize() {

		Logger::Initialize();

		data.windowSize = UVector2I(1280, 720);
		data.window = new Window(WindowData("Copper Engine", 1280, 720));
		data.fbo = new FrameBuffer(data.windowSize);

		Renderer::Initialize();
		UI::Initialize();
		Scripting::Initialize();
		
		Renderer::SetShader(new Shader("assets/Shaders/vertexDefault.glsl", "assets/Shaders/fragmentDefault.glsl"));

	}

	void Run() {

		while (data.running) {

			//Frame Begin
			data.fbo->Bind();
			data.scene->Update(data.renderScene);
			data.fbo->Unbind();

			data.EditorRun();

			UI::Begin();
			data.EditorUI();
			UI::End();

			//Frame End
			Renderer::EndFrame();
			data.window->Update();

		}

	}

	void Shutdown() {

		UI::Shutdown();
		data.window->Shutdown();

		//std::cin.get();

	}
	void LoadScene(Scene* scene) {

		data.scene = scene;
		
	}

	bool OnWindowResize(const Event& e) {

		return true;

	}
	bool OnWindowClose(const Event& e) {

		if (!data.EditorOnWindowClose(e)) return false;

		data.running = false;

		return true;

	}

	bool OnKeyPressed(const Event& e) {

		data.EditorOnKeyPressed(e);

		return true;

	}

	//Getters
	Window GetWindow() { return *data.window; }
	UVector2I GetWindowSize() { return data.windowSize; }

	uint32_t GetFBOTexture() { return data.fbo->GetColorAttachment(); }

	Scene* GetScene() { return data.scene; }
	Object GetObjectFromID(int32_t id) { return data.scene->GetRegistry()->GetObjectFromID(id); }

	//Setters
	void SetWindowSize(UVector2I size) {

		if (size.x == data.windowSize.x && size.y == data.windowSize.y) return;
		
		data.windowSize = size;

		data.fbo->Resize(data.windowSize);
		if (data.scene->cam) data.scene->cam->Resize(data.windowSize);
	
	}

	void SetRenderScene(bool value) { data.renderScene = value; }

	void SetEditorRunFunc(void (*func)()) { data.EditorRun = func; }
	void SetEditorUIFunc(void (*func)()) { data.EditorUI = func; }

	void SetEditorOnKeyPressedFunc(bool (*func)(const Event&)) { data.EditorOnKeyPressed = func; }
	void SetEditorOnWindowCloseFunc(bool (*func)(const Event&)) { data.EditorOnWindowClose = func; }

}
