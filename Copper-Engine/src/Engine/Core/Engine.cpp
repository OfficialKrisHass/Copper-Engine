#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"
#include "Engine/Core/SignalHandler.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/UI/ImGui.h"

#include "Engine/Input/AxisManager.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Components/Camera.h"

#include "Engine/Physics/PhysicsEngine.h"

#include "Engine/Scripting/ScriptingEngine.h"

#ifdef CU_EDITOR
extern Copper::Window* GetEditorWindow();
#endif

namespace Copper {

    namespace Renderer { void EndFrame(); }

    struct EngineData {

        // Core

        EngineState engineState = EngineState::Entry;

        // Renderer

#ifdef CU_EDITOR
        Window* window = nullptr; // Editor creates its own window and then passes it to the Engine
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
        Window& GetWindow() {

            CUP_FUNCTION();

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

    void EngineInitialize() {

        CUP_FUNCTION();

        VERIFY_STATE(EngineState::Entry, "Initialize the Engine");
        data.engineState = EngineState::Initialization;

        LogStatus("Initializing Copper-Engine.");

#ifdef CU_DEBUG
        SignalHandler::RegisterHandler(SignalHandler::Signal::Abort, Profiler::CrashHandler);
        SignalHandler::RegisterHandler(SignalHandler::Signal::Segfault, Profiler::CrashHandler);
#endif

        // Window & Renderer Initialization

#ifdef CU_EDITOR
        data.window = GetEditorWindow();
        CU_ASSERT(data.window != nullptr, "GetEditorWindow() returned nullptr, check if you have created a window in AppEntryPoint.");
#else
        data.window.Initialize("Copper Engine", 1280, 720);
#endif

        data.GetWindow().GetWindowCloseEvent() += OnWindowClose;
#ifndef CU_EDITOR
        data.GetWindow().GetWindowResizeEvent() += OnWindowResize;
#endif

        Renderer::Initialize();
        Renderer::SetShaderPath(ExecutableFolder() / "assets/Shaders/vertexDefault.glsl", ExecutableFolder() / "assets/Shaders/fragmentDefault.glsl");
        data.fbo.Create(UVector2I(1280, 720), { FrameBuffer::Attachment::Format::RGB8 }); // TODO: Find a solution to this (maybe store the resolution somewhere ?)
        LogStatus("\tMain frame buffer created.");

        data.mainUIContext.Initialize(data.GetWindow(), true);
        LogStatus("\tMain UI Context initialized.");

        // Input

        Input::Initialize(&data.GetWindow());
        Input::InitializeAxisManager();

        LogStatus("\tInput system initialized.");

        // Other systems initialization

        PhysicsEngine::Initialize();
        Scripting::Initialize();

        data.scene.Initialize();

        // Finalization
        
        LogStatus("Copper-Engine initialized.");

        data.engineState = EngineState::PostInitialization;
        data.postInitEvent();

    }
    void EngineRun() {

        CUP_FUNCTION();

        VERIFY_STATE(EngineState::PostInitialization, "Run the Engine");
        data.engineState = EngineState::Running;

        LogStatus("Entering engine run loop.");

        while (data.engineState == EngineState::Running) {

            CUP_START_FRAME(nullptr);

            // Calculate delta time

            float time = data.GetWindow().GetTime();
            data.deltaTime = time - data.lastFrameTime;
            data.lastFrameTime = time;

            // Update

            CUP_START_FRAME("Window");

            data.GetWindow().Update();

            CUP_END_FRAME();

            CUP_START_FRAME("Scene");

            data.fbo.Bind();
            data.scene.Update(data.deltaTime);
            data.fbo.Unbind();

            data.updateEvent();

            Renderer::EndFrame();
            RendererAPI::ResizeViewport(data.GetWindow().GetSize());

            CUP_END_FRAME();

            // UI Time window

            CUP_START_FRAME("UI");

            data.mainUIContext.Begin();
            data.uiUpdateEvent();
            data.mainUIContext.End();

            CUP_END_FRAME();

            Input::Update();

            CUP_END_FRAME(); // Main

        }

        LogStatus("Engine run loop exited.");

    }
    void EngineShutdown() {

        CUP_FUNCTION();

        VERIFY_STATE(EngineState::Shutdown, "Shutdown the Engine");

        LogStatus("Shutting down Copper-Engine.");

        data.mainUIContext.Shutdown();
        LogStatus("\tMain UI context shut down.");
        data.GetWindow().Shutdown();
        LogStatus("\tMain window shut down.");

        Scripting::Shutdown();
        PhysicsEngine::Shutdown();

        LogStatus("Copper-Engine shut down.");

        data.postShutdownEvent();

    }

    EngineState GetEngineState() { return data.engineState; }
    const char* EngineStateToString(EngineState state) {

        switch (state) {

            case EngineState::Entry: return "Entry"; break;
            case EngineState::Initialization: return "Initialization"; break;
            case EngineState::PostInitialization: return "Post Initialization"; break;
            case EngineState::Running: return "Running"; break;
            case EngineState::Shutdown: return "Shutdown"; break;

        }

        LogError("Invalid engine state: {}", static_cast<uint8>(state));
        return "Invalid Engine State!";

    }

    bool OnWindowClose(const Event& e) {

        CUP_FUNCTION();

        LogStatus("Window close event has been triggered.");

        if (!data.preShutdownEvent()) {

            LogStatus("Window close has been blocked.");
            return false;

        }
        data.engineState = EngineState::Shutdown;

        LogStatus("Window close event was succesfull, requesting shutdown.");

        return true;

    }
    bool OnWindowResize(const Event& e) {

        CUP_FUNCTION();

        // Editor handles resizing on its own

#ifndef CU_EDITOR
        data.fbo.Resize(data.GetWindow().GetSize());
        data.scene.GetMainCamera()->Resize(data.GetWindow().GetSize());
#endif

        return true;

    }

    // Engine Events

    SimpleEvent& GetPostInitEvent() { return data.postInitEvent; }

    SimpleEvent& GetUpdateEvent() { return data.updateEvent; }
    SimpleEvent& GetUIUpdateEvent() { return data.uiUpdateEvent; }

    Event& GetPreShutdownEvent() { return data.preShutdownEvent; }
    SimpleEvent& GetPostShutdownEvent() { return data.postShutdownEvent; }

    // Game

    float GetDeltaTime() { return data.deltaTime; }

    // Declaration in Window.h

    Window& GetWindow() { return data.GetWindow(); }
    UVector2I GetWindowSize() {

#ifdef CU_EDITOR
        return data.fbo.GetSize();
#else
        return data.window.GetSize();
#endif

    }
    float GetWindowAspectRatio() {

#ifdef CU_EDITOR
        return static_cast<float>(data.fbo.GetSize().x) / data.fbo.GetSize().y;
#else
        return data.window.GetAspectRatio();
#endif

    }

    void SetMainWindowAsCurrent() { data.GetWindow().SetAsCurrentContext(); }
    void SetWindowSize(const UVector2I& size) {

        CUP_FUNCTION();

#ifdef CU_EDITOR
        if (data.fbo.GetSize() == size) return;

        data.fbo.Resize(size);
        data.scene.GetMainCamera()->Resize(size);
#else
        data.window.SetSize(size);
#endif

    }

    // Declaration in FrameBuffer.h

    const FrameBuffer& GetMainFBO() { return data.fbo; }

    // Declaration in ImGui.h

    const UIContext& MainUIContext() { return data.mainUIContext; }

    // Declaration in Scene.h

    Scene* GetScene() { return &data.scene; }

}
