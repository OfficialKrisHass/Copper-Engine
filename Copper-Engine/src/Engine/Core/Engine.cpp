#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#include "Engine/UI/ImGui.h"

#include "Engine/Input/AxisManager.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Physics/PhysicsEngine.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Components/Camera.h"

#ifdef CU_EDITOR
extern Copper::Window* GetEditorWindow();
#endif

#define VERIFY_STATE_INTERNAL(state, task) CHECK(data.engineState == state, "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineStateToString(state), EngineStateToString(data.engineState))

namespace Copper {

	using namespace EngineCore;

	struct EngineData {

		// Core variables

		EngineState engineState = EngineState::Entry;
		std::vector<std::string> arguments;

		//Rendering
	
	#ifdef CU_EDITOR // Editor creates its own window and then passes it to the Engine
		Window* window;
		bool acceptInputRuntime = true;
	#else // Every other app lets the Engine create the window
		Window window;
	#endif
		FrameBuffer fbo;
		UIContext mainUIContext;

		// Scene

		Scene scene;
		float lastFrameTime = 0.0f;
		bool renderScene = true;

		// Engine Events

		SimpleEvent postInitEvent;
		SimpleEvent updateEvent;
		SimpleEvent uiUpdateEvent;
		Event preShutdownEvent;
		SimpleEvent postShutdownEvent;

		// Helper function to get a reference to the window no matter if we are using the editor or not
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

	void Run();
	void Shutdown();

#pragma region EngineCore
	void EngineCore::Initialize(int argc, char* argv[]) {

		// This is the only function that has to do this as this is the only exposed function
		VERIFY_STATE_INTERNAL(EngineState::Entry, "Initialize the Engine");
		data.engineState = EngineState::Initialization;

		// TODO: Implement our own argument system
		for (int i = 0; i < argc; i++) {

			if (Filesystem::Path(argv[i]).Extension() == ".exe") continue;

			data.arguments.push_back(argv[i]);

		}

		Logger::Initialize();

		// Window & Renderer Initialization

	#ifdef CU_EDITOR
		data.window = GetEditorWindow();
		RendererAPI::Initialize();
		data.fbo = FrameBuffer(UVector2I(1280, 720)); // TODO: Possibly find a solution that doesnt mean we have to have an invalid size for the first few frames
	#else
		data.window = Window("Copper Engine", 1280, 720);
		RendererAPI::Initialize();
		data.fbo = FrameBuffer(data.WindowRef().Size());
	#endif

		data.WindowRef().AddWindowCloseEventFunc(OnWindowClose);
		data.WindowRef().AddWindowResizeEventFunc(OnWindowResize);

		Renderer::Initialize();
		Renderer::SetShader(Shader("assets/Shaders/vertexDefault.glsl", "assets/Shaders/fragmentDefault.glsl"));
		data.mainUIContext.Initialize(data.WindowRef(), true);

		// Input

		Input::Init();
		Input::InitializeAxisManager();

		// Physics

		PhysicsEngine::Initialize();

		// Scripting

		Scripting::Initialize();

		// Finalization

		data.engineState = EngineState::PostInitialization;
		data.postInitEvent();

		Run();

	}
	void Run() {

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

			data.mainUIContext.Begin();
			data.uiUpdateEvent();
			data.mainUIContext.End();

			Renderer::EndFrame();

			Input::Update();

		}

		Shutdown();

	}
	void Shutdown() {

		data.mainUIContext.Shutdown();
		data.WindowRef().Shutdown();
		data.postShutdownEvent();

	}

	EngineState EngineCore::GetEngineState() { return data.engineState; }
	std::string EngineCore::EngineStateToString(EngineState state) {

		switch (state) {

			case EngineState::Entry: return "Entry"; break;
			case EngineState::Initialization: return "Initialization"; break;
			case EngineState::PostInitialization: return "Post Initialization"; break;
			case EngineState::Running: return "Running"; break;
			case EngineState::Shutdown: return "Shutdown"; break;

		}

		return "Invalid Engine State!";

	}

	uint32_t EngineCore::GetNumArguments() { return (uint32_t) data.arguments.size(); }
	const std::string& EngineCore::GetArgument(uint32_t index) { return data.arguments[index]; }
#pragma endregion

	bool OnWindowClose(const Event& e) {

		if (!data.preShutdownEvent()) return false;
		data.engineState = EngineState::Shutdown;

		return true;

	}
	bool OnWindowResize(const Event& e) {

		// Editor handles resizing on its own

	#ifndef CU_EDITOR
		data.fbo.Resize(data.WindowRef().Size());
		data.scene.cam->Resize(data.WindowRef().Size());
	#endif

		return true;

	}

	// Engine Events
	
	void AddPostInitEventFunc(std::function<void()> func) { data.postInitEvent += func; }

	void AddUpdateEventFunc(std::function<void()> func) { data.updateEvent += func; }
	void AddUIUpdateEventFunc(std::function<void()> func) { data.uiUpdateEvent += func; }

	void AddPreShutdownEventFunc(std::function<bool(const Event&)> func) { data.preShutdownEvent += func; }
	void AddPostShutdownEventFunc(std::function<void()> func) { data.postShutdownEvent += func; }

	// Declaration in Window.h

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

	void SetMainWindowAsCurrent() { data.WindowRef().SetAsCurrentContext(); }
	void SetWindowSize(const UVector2I& size) {

	#ifdef CU_EDITOR
		if (data.fbo.Size() == size) return;

		data.fbo.Resize(size);
		data.scene.cam->Resize(size);
	#else
		data.window.SetSize(size);
	#endif
	
	}

	// Declaration in FrameBuffer.h

	uint32_t GetMainFBOTexture() { return data.fbo.GetColorAttachment(); }

	// Declaration in ImGui.h

	void SetMainUIAsCurrent() { data.mainUIContext.SetAsCurrent(); }
	void LoadMainUIContextFont(const std::string& path, float fontSize) { data.mainUIContext.LoadFont(path, fontSize); }

	// Declaration in Scene.h
	
	Scene* GetScene() { return &data.scene; }
	void SetShouldRenderScene(bool value) { data.renderScene = value; }

	uint32_t GetNumOfEntities() { return data.scene.GetNumOfEntities(); }
	bool IsSceneRuntimeRunning() { return data.scene.IsRuntimeRunning(); }

	InternalEntity* CreateEntity(ENTITY_PROPERTIES_DECLARATION) { return data.scene.CreateEntity(position, rotation, scale, name); }
	InternalEntity* CreateEntityFromID(uint32_t id, ENTITY_PROPERTIES_DECLARATION, bool returnIfExists) { return data.scene.CreateEntityFromID(id, position, rotation, scale, name, returnIfExists); }
	InternalEntity* GetEntityFromID(uint32_t id) { return data.scene.GetEntityFromID(id); }
	void RemoveEntity(InternalEntity* entity) { data.scene.RemoveEntity(entity); }
	void RemoveEntityFromID(uint32_t id) { data.scene.RemoveEntityFromID(id); }

// Editor misc.
#ifdef CU_EDITOR
	void SetAcceptInputDuringRuntime(bool value) { data.acceptInputRuntime = value; }
	bool AcceptInputDuringRuntime() { return data.acceptInputRuntime; }
#endif

}
