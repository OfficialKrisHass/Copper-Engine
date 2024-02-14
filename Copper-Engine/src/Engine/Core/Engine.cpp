#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"
#include "Engine/Core/Args.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#include "Engine/UI/ImGui.h"

#include "Engine/Input/AxisManager.h"

#include "Engine/Scene/CopperECS.h"
#include "Engine/Components/Camera.h"

#include "Engine/Physics/PhysicsEngine.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Debug/Profiler.h"

#ifdef CU_EDITOR
extern Copper::Window* GetEditorWindow();
#endif

#define VERIFY_STATE_INTERNAL(state, task) CHECK(data.engineState == state, "Cannot {} because of invalid Engine State.\nExpected State: {}\nCurrent State: {}", task, EngineStateToString(state), EngineStateToString(data.engineState))

namespace Copper {

	using namespace EngineCore;
	namespace Renderer { void EndFrame(); }

	struct EngineData {

		// Core

		EngineState engineState = EngineState::Entry;

		// Renderer
	
	#ifdef CU_EDITOR
		Window* window = nullptr; // Editor creates its own window and then passes it to the Engine
		bool acceptInputRuntime = true;
	#else
		Window window;
	#endif
		FrameBuffer fbo = FrameBuffer();
		UIContext mainUIContext = UIContext();

		// Scene

		Scene scene;

		float lastFrameTime = 0.0f;
		float deltaTime = 0.0f;

		// Engine Events

		SimpleEvent postInitEvent;
		SimpleEvent updateEvent;
		SimpleEvent uiUpdateEvent;
		Event preShutdownEvent;
		SimpleEvent postShutdownEvent;

		// Helper func to get the Window without macros everywhere
		Window& Window() {

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
	void EngineCore::Initialize() {

		CUP_FUNCTION();

		VERIFY_STATE_INTERNAL(EngineState::Entry, "Initialize the Engine");
		data.engineState = EngineState::Initialization;

		// Window & Renderer Initialization

	#ifdef CU_EDITOR
		data.window = GetEditorWindow();
		CU_ASSERT(data.window, "Editor Window returned nullptr! Check if you have created a window in AppEntryPoint and provided a GetEditorWindow function!");
	#else
		data.window = Window("Copper Engine", 1280, 720);
	#endif

		data.Window().AddWindowCloseEventFunc(OnWindowClose);
		data.Window().AddWindowResizeEventFunc(OnWindowResize);

		Renderer::Initialize();
		Renderer::SetShaderPath("assets/Shaders/vertexDefault.glsl", "assets/Shaders/fragmentDefault.glsl");
		data.fbo = FrameBuffer(UVector2I(1280, 720)); // TODO: Find a solution to this (maybe store the resolution somewhere ?)

		data.mainUIContext.Initialize(data.Window(), true);

		// Input

		Input::Initialize(data.Window());
		Input::InitializeAxisManager();

		// Physics

		PhysicsEngine::Initialize();

		// Scripting

		Scripting::Initialize();

		// Finalization

		data.engineState = EngineState::PostInitialization;
		data.postInitEvent();

		// Call it from here so that it doesn't have to be an exposed function
		Run();

	}
	void Run() {

		CUP_FUNCTION();

		data.engineState = EngineState::Running;

		while (data.engineState == EngineState::Running) {

			// Calculate delta time

			float time = data.Window().Time();
			data.deltaTime = time - data.lastFrameTime;
			data.lastFrameTime = time;

			// Update

			data.Window().Update();
			data.updateEvent();

			data.fbo.Bind();
			data.scene.Update(data.deltaTime);
			data.fbo.Unbind();

			RendererAPI::ResizeViewport(data.Window().Size());

			// UI Time window

			data.mainUIContext.Begin();
			data.uiUpdateEvent();
			data.mainUIContext.End();

			// Finalize

			Renderer::EndFrame();

			Input::Update();

		}

		// Again call this from here so it doesn't have to be exposed

		Shutdown();

	}
	void Shutdown() {

		CUP_FUNCTION();

		data.mainUIContext.Shutdown();
		data.Window().Shutdown();

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

#pragma endregion

	bool OnWindowClose(const Event& e) {

		CUP_FUNCTION();

		if (!data.preShutdownEvent()) return false;
		data.engineState = EngineState::Shutdown;

		return true;

	}
	bool OnWindowResize(const Event& e) {

		// Editor handles resizing on its own

	#ifndef CU_EDITOR
		data.fbo.Resize(data.Window().Size());
		data.scene.cam->Resize(data.Window().Size());
	#endif

		return true;

	}

	// Engine Events
	
	void AddPostInitEventFunc(std::function<void()> func) { data.postInitEvent += func; }

	void AddUpdateEventFunc(std::function<void()> func) { data.updateEvent += func; }
	void AddUIUpdateEventFunc(std::function<void()> func) { data.uiUpdateEvent += func; }

	void AddPreShutdownEventFunc(std::function<bool(const Event&)> func) { data.preShutdownEvent += func; }
	void AddPostShutdownEventFunc(std::function<void()> func) { data.postShutdownEvent += func; }

	// Game

	float DeltaTime() { return data.deltaTime; }

	// Declaration in Window.h

	Window& GetWindow() { return data.Window(); }
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

	void SetMainWindowAsCurrent() { data.Window().SetAsCurrentContext(); }
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

	uint32 GetMainFBOTexture() { return data.fbo.ColorTextureID(); }

	// Declaration in ImGui.h

	void SetMainUIAsCurrent() { data.mainUIContext.SetAsCurrent(); }
	void LoadMainUIContextFont(const std::string& path, float fontSize) { data.mainUIContext.LoadFont(path, fontSize); }

	// Declaration in Scene.h
	
	Scene* GetScene() { return &data.scene; }

	uint32 GetNumOfEntities() { return data.scene.GetNumOfEntities(); }
	bool IsSceneRuntimeRunning() { return data.scene.IsRuntimeRunning(); }

	InternalEntity* CreateEntity(ENTITY_PROPERTIES_DECLARATION) { return data.scene.CreateEntity(position, rotation, scale, name); }
	InternalEntity* CreateEntityFromID(uint32 id, ENTITY_PROPERTIES_DECLARATION, bool returnIfExists) { return data.scene.CreateEntityFromID(id, position, rotation, scale, name, returnIfExists); }
	InternalEntity* GetEntityFromID(uint32 id) { return data.scene.GetEntityFromID(id); }
	void RemoveEntity(InternalEntity* entity) { data.scene.RemoveEntity(entity); }
	void RemoveEntityFromID(uint32 id) { data.scene.RemoveEntityFromID(id); }

// Editor misc.
#ifdef CU_EDITOR
	void SetAcceptInputDuringRuntime(bool value) { data.acceptInputRuntime = value; }
	bool AcceptInputDuringRuntime() { return data.acceptInputRuntime; }
#endif

}
