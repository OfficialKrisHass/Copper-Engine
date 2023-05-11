#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Input/AxisManager.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/UI/ImGui.h"

#include "Engine/Scene/CopperECS.h"

#include <ImGui/imgui.h>
#include <glad/glad.h>

#ifdef CU_EDITOR
extern Copper::Window* GetEditorWindow();
#endif

namespace Copper {

	struct EngineData {

		Version version;
		EngineState engineState = EngineState::Entry;
		std::vector<std::string> arguments;

		FrameBuffer fbo;
	#ifdef CU_EDITOR
		Window* window; //Physical Window - Editor creates, sets and stores the window
		bool acceptInputRuntime = true;
	#else
		Window window;
	#endif

		Scene scene;
		float lastFrameTime = 0.0f;
		bool renderScene = true;

		SimpleEvent postInitEvent;
		SimpleEvent updateEvent;
		SimpleEvent uiUpdateEvent;
		Event preShutdownEvent;
		SimpleEvent postShutdownEvent;

		Window& WindowRef() {

		#ifdef CU_EDITOR
			return *window;
		#else
			return window;
		#endif

		}
			
	};
	EngineData data;

	bool OnWindowClose(const Event& e);
	bool OnWindowResize(const Event& e);

	void EngineCore::Initialize(int argc, char* argv[]) {

		CHECK((data.engineState == EngineState::Entry), "Cannot Initialize the Engine, current Engine State is: {}", EngineStateToString(data.engineState));
		
		data.version = Version(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TWEAK);
		data.version.sceneVersion = SCENE_VERSION;
		data.engineState = EngineState::Initialization;
		for (int i = 0; i < argc; i++) {

			if (Filesystem::Path(argv[i]).Extension() == ".exe") continue;

			data.arguments.push_back(argv[i]);

		}

		Logger::Initialize();

	#ifdef CU_EDITOR
		data.window = GetEditorWindow();
		RendererAPI::Initialize();
		data.fbo = FrameBuffer(UVector2I(1280, 720));
	#else
		data.window = Window("Copper Engine", 1280, 720);
		RendererAPI::Initialize();
		data.fbo = FrameBuffer(data.WindowRef().Size());
	#endif

		data.WindowRef().AddWindowCloseEventFunc(OnWindowClose);
		data.WindowRef().AddWindowResizeEventFunc(OnWindowResize);

		Renderer::Initialize();
		UI::Initialize();
		Renderer::SetShader(Shader("assets/Shaders/vertexDefault.glsl", "assets/Shaders/fragmentDefault.glsl"));

		Input::Init();

		Input::AddAxis("Keys_WS", KeyCode::W, KeyCode::S);
		Input::AddAxis("Keys_DA", KeyCode::D, KeyCode::A);

		Input::AddMouseAxis("Mouse X", true);
		Input::AddMouseAxis("Mouse Y", false);

		Scripting::Initialize();

		data.engineState = EngineState::PostInitialization;
		data.postInitEvent();

	}

	void EngineCore::Run() {

		CHECK((data.engineState == EngineState::PostInitialization), "Cannot Start running the Engine, current Engine State is: {}", EngineStateToString(data.engineState));
		data.engineState = EngineState::Running;

		while (data.engineState == EngineState::Running) {

			float time = data.WindowRef().Time();
			float deltaTime = time - data.lastFrameTime;
			data.lastFrameTime = time;

			data.WindowRef().Update();

			data.updateEvent();

			data.fbo.Bind();
			data.scene.Update(data.renderScene, deltaTime);
			data.fbo.Unbind();

			Renderer::ResizeViewport(data.WindowRef().Size());

			UI::Begin();
			data.uiUpdateEvent();
			UI::End();

			Renderer::EndFrame();

			Input::Update();

		}

	}

	void EngineCore::Shutdown() {

		CHECK((data.engineState == EngineState::Shutdown), "Cannot Shutdown the Engine, current Engine State is: {}", EngineStateToString(data.engineState));

		UI::Shutdown();
		data.WindowRef().Shutdown();

		data.postShutdownEvent();

	}

	bool OnWindowClose(const Event& e) {

		if (!data.preShutdownEvent()) return false;
		data.engineState = EngineState::Shutdown;

		return true;

	}
	bool OnWindowResize(const Event& e) {

	#ifndef CU_EDITOR
		data.fbo.Resize(data.WindowRef().Size());
		data.scene.cam->Resize(data.WindowRef().Size());
	#endif

		return true;

	}

	SimpleEvent& GetPostInitEvent() { return data.postInitEvent; }

	SimpleEvent& GetUpdateEvent() { return data.updateEvent; }
	SimpleEvent& GetUIUpdateEvent() { return data.uiUpdateEvent; }

	Event& GetPreShutdownEvent() { return data.preShutdownEvent; }
	SimpleEvent& GetPostShutdownEvent() { return data.postShutdownEvent; }

	//Setters
	void SetWindowSize(const UVector2I& size) {

	#ifdef CU_EDITOR
		if (data.fbo.Size() == size) return;

		data.fbo.Resize(size);
		data.scene.cam->Resize(size);
	#else
		data.window.SetSize(size);
	#endif
	
	}

	void SetRenderScene(bool value) { data.renderScene = value; }

	//Getters
	Window& GetWindow() { return data.WindowRef(); }
	UVector2I GetWindowSize() {
		
	#ifdef CU_EDITOR
		return data.fbo.Size();
	#else
		return data.window.Size();
	#endif

	}
	float GetWindowAspectRatio() {

	#ifdef CU_EDITOR
		return static_cast<float>(data.fbo.Width()) / data.fbo.Height();
	#else
		return data.window.AspectRatio();
	#endif

	}

#ifdef CU_EDITOR
	void SetAcceptInputDuringRuntime(bool value) { data.acceptInputRuntime = value; }
#endif

	uint32_t GetFBOTexture() { return data.fbo.GetColorAttachment(); }

	Scene* GetScene() { return &data.scene; }
	uint32_t GetNumOfEntities() { return data.scene.GetNumOfEntities(); }

	InternalEntity* GetEntityFromID(int32_t id) { return data.scene.GetEntityFromID(id); }
	InternalEntity* CreateEntityFromID(int32_t id, const std::string& name, bool returnIfExists) { return data.scene.CreateEntityFromID(id, Vector3::zero, Vector3::zero, Vector3::one, name, returnIfExists); }

	bool IsRuntimeRunning() { return data.scene.IsRuntimeRunning(); }

	void AddPostInitEventFunc(std::function<void()> func) { data.postInitEvent += func; }

	void AddUpdateEventFunc(std::function<void()> func) { data.updateEvent += func; }
	void AddUIUpdateEventFunc(std::function<void()> func) { data.uiUpdateEvent += func; }

	void AddPreShutdownEventFunc(std::function<bool(const Event&)> func) { data.preShutdownEvent += func; }
	void AddPostShutdownEventFunc(std::function<void()> func) { data.postShutdownEvent += func; }

	const Version& GetVersion() { return data.version; }

	EngineState GetEngineState() { return data.engineState; }
	std::string EngineStateToString(EngineState state) {

		switch (state) {

			case EngineState::Entry: return "Entry"; break;
			case EngineState::Initialization: return "Initialization"; break;
			case EngineState::PostInitialization: return "Post Initialization"; break;
			case EngineState::Running: return "Running"; break;
			case EngineState::Shutdown: return "Shutdown"; break;

		}

		return "Invalid Engine State!";

	}

	uint32_t GetNumArguments() { return (uint32_t) data.arguments.size(); }
	const std::string& GetArgument(uint32_t index) { return data.arguments[index]; }

#ifdef CU_EDITOR
	bool AcceptInputDuringRuntime() {

		return data.acceptInputRuntime;

	}
#endif

}
