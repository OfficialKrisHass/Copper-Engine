#include "EditorApp.h"

#include "Core/SceneSerializer.h"
#include "Core/FieldSerializer.h"
#include "Core/ChangeHandler.h"
#include "Core/Clipboard.h"

#include "Assets/ProjectAssetDatabase.h"

#include "Projects/Project.h"
#include "Projects/ProjectTemplate.h"

#include "Panels/Viewport.h"
#include "Panels/CamView.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"
#include "Panels/Console.h"
#include "Panels/ProjectSettings.h"
#include "Panels/Profiler.h"
#include "Panels/ThemeEditor.h"

#include "Viewport/SceneCamera.h"

#include <Engine/Core/Args.h>
#include <Engine/Core/Core.h>

#include <Engine/Core/Log.h>

#include <Engine/Renderer/FrameBuffer.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/Texture.h>

#include <Engine/Scripting/ScriptingEngine.h>

#include <Engine/Input/Input.h>
#include <Engine/Input/Popup.h>

#include <Engine/UI/ImGui.h>

#include <Engine/Utilities/FileUtils.h>

#include <CMath/CMath.h>

#include <GLM/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

#include <yaml-cpp/yaml.h>

#include <fstream>

using namespace Copper;

// This file contains like 99% of the entire editor...... WHYYYYYYYYYYY ??????????
// TODO: Fix this you stupid dumbass (me)
namespace Editor {

    struct EditorData {

        // Core Data

        EditorState state = EditorState::Edit;
        Window window;
        std::string title;
        bool interactionBlocked = false;

        // Project

        Project project;
        fs::path projectToLoad;
        bool shouldCreateNewProject = false;

        // Scene

        Scene* scene = nullptr;
        fs::path scenePath;
        fs::path nextScenePath;

        // Game Panel

        UVector2I gamePanelSize = UVector2I(1280, 720);
        bool gamePanelFocused = false;

        // Icons

        Texture playIcon;
        Texture stopIcon;
        
        // Panels

        Viewport viewport;
        CamView camView;
        SceneHierarchy sceneHierarchy;
        Properties properties;
        FileBrowser fileBrowser;
        Console console;
        ThemeEditor themeEditor;

        // Theme Editor

        bool themeEditorOpen = false;

        // Debug only
#ifdef CU_DEBUG
        bool imguiDemoWindow = false;
#endif

    };

    EditorData data;

    void QueuedTasks();

    void LoadEditorData();
    void SaveEditorData();

    void RenderDockspace();
    void RenderToolbar();
    void RenderMenu();

    void NewProject();

    void CopyScriptingAPI();

    void StartEditorRuntime();
    void StopEditorRuntime();

    bool OnKeyPressed(const Event& e);
    bool OnWindowClose(const Event& e);
    bool OnWindowFocused(const Event& e);

    void Initialize() {

        CUP_FUNCTION();

        LogStatus("Initializing Copper-Editor.");

        data.window.GetKeyPressedEvent() += Editor::OnKeyPressed;
        data.window.GetWindowFocusedEvent() += Editor::OnWindowFocused;

        GetMainUIContext().SetDefaultFont(ExecutableFolder() / "assets/Fonts/IBMPlexMono-Medium.ttf");

        data.scene = GetScene();

        data.viewport.Initialize();
        data.fileBrowser.Initialize();
        data.console.Initialize();

        data.themeEditor.LoadTheme(ExecutableFolder() / "assets/Themes/Default.cutheme");

        data.playIcon.Create(ExecutableFolder() / "assets/Icons/PlayButton.png", Texture::Format::RGBA);
        data.stopIcon.Create(ExecutableFolder() / "assets/Icons/StopButton.png", Texture::Format::RGBA);

        LoadEditorData();

#ifdef CU_LINUX
        data.project.RunPremake();
#endif

        LogStatus("Copper-Editor initialized.");

    }
    void Shutdown() {

        CUP_FUNCTION();

        LogStatus("Shutting down Copper-Editor.");

        ProjectAssetDatabase::Shutdown();

        data.project.Save(false);
        SaveEditorData();

        LogStatus("Copper-Editor shut down.");

    }

    void SaveEditorData() {

        CUP_FUNCTION();

        YAML::Emitter out;

        out << YAML::BeginMap; //Start

        out << YAML::Key << "Last Project" << YAML::Value << data.project.GetPath();

        out << YAML::Key << "Gizmo operation" << YAML::Value << static_cast<uint16>(data.viewport.GetGizmo().operation);
        out << YAML::Key << "Gizmo global mode" << YAML::Value << data.viewport.GetGizmo().globalMode;

        out << YAML::EndMap; //End

        std::ofstream file(ExecutableFolder() / "assets/EditorData.cu");
        file << out.c_str();

    }
    void LoadEditorData() {

        CUP_FUNCTION();

        // Load the file

        if (!fs::exists(ExecutableFolder() / "assets/EditorData.cu")) {

            LogWarn("EditorData.cu is missing, generating a default one");
            SaveEditorData();

        }

        YAML::Node main;
        try { main = YAML::LoadFile((ExecutableFolder() / "assets/EditorData.cu").string()); }
        catch (YAML::Exception e) {

            Input::ErrorPopup("EditorData.cu read failed", "Could not read the EditorData.cu file.\n\nIt should be located: " + (ExecutableFolder() / "assets/EditorData.cu").string() + "\n\nError message: " + e.what());
            exit(-1);

        }

        // Gizmo

        data.viewport.SetGizmoOperation(static_cast<Viewport::Gizmo::Operation>(main["Gizmo operation"].as<uint16>()));
        data.viewport.SetGizmoGlobalMode(main["Gizmo global mode"].as<bool>());

        // Project

        // If a project was passed as an argument, it has priority.
        if (!Args::GetProjectToOpenPath().empty()) {

            data.project.Open(Args::GetProjectToOpenPath());
            return;

        }

        std::string path = main["Last Project"].as<std::string>();
        if (!std::filesystem::exists(path)) {

            switch (Input::WarningPopup("Last opened project error", "The last opened project no longer exists, do you wish to open a project manually, or exit the Editor ?\n\nLast opened project: " + path)) {

                case Input::PopupResult::Yes: data.project.Open(); return;
                case Input::PopupResult::No: exit(-1);
                default: exit(1);

            }

        }

        data.project.Open(path);

    }

    void Update() {

        CUP_FUNCTION();
        CUP_START_FRAME("Editor");

        data.project.Update();
        
        data.viewport.Update();

        QueuedTasks();

        CUP_END_FRAME();

    }
    void UIUpdate() {
        
        CUP_START_FRAME("Editor UI");

        RenderDockspace();
        RenderMenu();
        
        data.console.UIRender();
        data.fileBrowser.UIRender();
        data.properties.UIRender();
        data.sceneHierarchy.UIRender();
        data.viewport.UIRender();
        data.camView.UIRender();

        if (data.themeEditorOpen)
            data.themeEditor.UIRender();

        ProjectSettings::UIRender();
        Profiler::UIRender();

        RenderToolbar();

#ifdef CU_DEBUG
        if (data.imguiDemoWindow)
            ImGui::ShowDemoWindow(&data.imguiDemoWindow);
#endif

        ImGui::End(); //Dockspace

        CUP_END_FRAME();

    }

    void QueuedTasks() {

        CUP_FUNCTION();

        if (!data.nextScenePath.empty()) {

            OpenScene(data.nextScenePath);
            data.nextScenePath.clear();

        }
        if (!data.projectToLoad.empty()) {

            data.project.Open(data.projectToLoad);
            data.projectToLoad.clear();

        }
        if (data.shouldCreateNewProject) {

            NewProject();
            data.shouldCreateNewProject = false;

        }

    }

    void RenderDockspace() {

        CUP_FUNCTION();

        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();

        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;

        ImGuiID dockspace_id = ImGui::GetID("Copper-EditorDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        style.WindowMinSize.x = minWinSizeX;

    }
    
    void RenderToolbar() {

        CUP_FUNCTION();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

        auto& colors = ImGui::GetStyle().Colors;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(colors[ImGuiCol_ButtonHovered].x, colors[ImGuiCol_ButtonHovered].y, colors[ImGuiCol_ButtonHovered].z, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(colors[ImGuiCol_ButtonActive].x, colors[ImGuiCol_ButtonActive].y, colors[ImGuiCol_ButtonActive].z, 0.5f));

        ImGuiWindowClass windowClass;
        windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&windowClass);
        ImGui::Begin("##ToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec2 buttonSize = ImVec2(25, 25);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - buttonSize.x) * 0.5f);

        if (data.state == EditorState::Edit) {

            if (ImGui::ImageButton("##StartButton", static_cast<ImTextureID>((uint64) data.playIcon.GetID()), buttonSize, {0, 1}, {1, 0}) && data.project.IsValid())
                StartEditorRuntime();

        } else if (data.state == EditorState::Play) {

            if (ImGui::ImageButton("##StopButton", static_cast<ImTextureID>((uint64) data.stopIcon.GetID()), buttonSize, {0, 1}, {1, 0}) && data.project.IsValid())
                StopEditorRuntime();

        }

        // Wireframe toggle
        
        bool wireframe = Renderer::GetWireframe();

        ImGui::SameLine();
        if (ImGui::Checkbox("Wireframe", &wireframe))
            Renderer::SetWireframe(wireframe);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();

    }
    void RenderMenu() {

        CUP_FUNCTION();

        if(ImGui::BeginMenuBar()) {

            if (ImGui::BeginMenu("Project")) {

                if (ImGui::MenuItem("New Project"))
                    data.shouldCreateNewProject = true;
                if (ImGui::MenuItem("Open Project")) {

                    fs::path path = Utilities::FolderOpenDialog("Open Project", data.project.GetPath().empty() ? ROOT_DIR : data.project.GetPath().parent_path());
                    if (!path.empty() && path != data.project.GetPath())
                        data.projectToLoad = path;

                }
                if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S", false, data.project.IsValid())) {

                    data.project.Save();
                    SaveEditorData();

                }

                ImGui::Separator();

                if (ImGui::MenuItem("Create Template", 0, false, data.project.IsValid()))
                    CreateTemplateFromProject(data.project);

                ImGui::Separator();

                if (ImGui::MenuItem("Build Scripts", "Ctrl+B", false, data.project.IsValid()))
                    data.project.Build();

                ImGui::Separator();

                if (ImGui::MenuItem("Project Settings"))
                    ProjectSettings::Open();
                
                ImGui::Separator();

                ImGui::EndMenu();

            }

            if(ImGui::BeginMenu("File")) {

                if(ImGui::MenuItem("New Scene", 0, false, data.project.IsValid()))
                    NewScene();
                if(ImGui::MenuItem("Open Scene", 0, false, data.project.IsValid()))
                    OpenSceneNext();
                if(ImGui::MenuItem("Save Scene", "Ctr+S", false, data.scene->IsInitialized()))
                    SaveScene();
                if(ImGui::MenuItem("Save Ass", "Ctrl+Alt+S", false, data.scene->IsInitialized()))
                    SaveSceneAs();

                ImGui::EndMenu();

            }

            if (ImGui::BeginMenu("Theme")) {

                if (ImGui::MenuItem("Open Theme Editor"))
                    data.themeEditorOpen = true;

                if (ImGui::MenuItem("Save Theme", 0, false)) {

                    fs::path path = Utilities::SaveDialog("Save Theme", { "Copper Editor Theme Files (.cutheme)", "*.cutheme" }, "assets\\Themes");
                    if (!path.empty())
                        data.themeEditor.SaveTheme(path);

                }
                if (ImGui::MenuItem("Load Theme", 0, false)) {

                    fs::path path = Utilities::OpenDialog("Load Theme", { "Copper Editor Theme Files (.cutheme)", "*.cutheme" }, "assets/Themes");
                    if (!path.empty())
                        data.themeEditor.LoadTheme(path);

                }

                ImGui::EndMenu();

            }

            if(ImGui::BeginMenu("Camera")) {

                if (ImGui::DragFloat("Speed", &data.viewport.GetSceneCamera().speed, 0.01f, 0.001f, 50.0f, "%.4f"))
                    SetChanges();
                if (ImGui::DragFloat("Sensitivity", &data.viewport.GetSceneCamera().sensitivity, 0.1f, 1.0f, 1000.0f))
                    SetChanges();

                ImGui::EndMenu();

            }

            if (ImGui::BeginMenu("Debug")) {

                if (ImGui::MenuItem("Profiler"))
                    Profiler::Open();

#ifdef CU_DEBUG
                ImGui::Separator();

                if (ImGui::MenuItem("Copy Copper Scripting API", 0, false, data.project.IsValid()))
                    CopyScriptingAPI();

                if (ImGui::MenuItem("ImGui demo Window"))
                    data.imguiDemoWindow = true;
#endif

                ImGui::EndMenu();

            }

            ImGui::EndMenuBar();

        }

    }

    void StartEditorRuntime() {

        CUP_FUNCTION();

        data.state = EditorState::Play;

        SceneSerializer::Serialize(data.scene, ExecutableFolder() / "assets/Temp/scene_lock.copper");

        Renderer::Restart();

        data.scene->DeinitializePhysics();
        data.scene->InitializePhysics();

    }
    void StopEditorRuntime() {

        CUP_FUNCTION();

        data.state = EditorState::Edit;

        data.scene->Deinitialize();
        SceneSerializer::Deserialize(data.scene, ExecutableFolder() / "assets/Temp/scene_lock.copper");
        data.scene->Initialize();

        data.camView.SetIsAcceptingInput(false);

        Input::SetCursorLocked(false);
        Input::SetCursorVisible(true);

    }

    void NewProject() {

        CUP_FUNCTION();

        fs::path path = Utilities::FolderOpenDialog("New Project", data.project.IsValid() ? data.project.GetPath().parent_path() : ROOT_DIR);
        if (path.empty()) return;

        // Create the Project

        data.project.Create(path);

        FileBrowser::Refresh();

#ifdef CU_LINUX
        data.project.RunPremake();
#endif
        data.project.Build();

        data.scene->Deinitialize();
        SceneSerializer::Deserialize(data.scene, path / "Assets" / data.project.GetLastOpenedScenePath());
        data.scene->Initialize();

        SceneHierarchy::SetScene(data.scene);
        Properties::ClearSelectedData();

        ClearChanges();

        data.title = "Copper Editor - " + data.project.name + ": Main.copper";
        data.window.SetTitle(data.title);

    }

    void CopyScriptingAPI() {

        CUP_FUNCTION();

        std::ifstream dllSrc(ExecutableFolder() / "assets/Copper-ScriptingAPI.dll", std::ios::binary);
        std::fstream dllDst;

        dllDst.open(data.project.GetPath() / "Binaries/Copper-ScriptingAPI.dll", std::ios::out | std::ios::binary);
        dllDst << dllSrc.rdbuf();
        dllDst.close();

    }

    void NewScene() {

        CUP_FUNCTION();

        data.scenePath = "";
        *data.scene = Scene();

        SceneHierarchy::SetScene(data.scene);
        Properties::ClearSelectedData();
        
    }
    void OpenScene(const fs::path& path, bool checkUnsavedChanges) {

        CUP_FUNCTION();

        CU_ASSERT(fs::exists(path), "Scene at path '{}' does not exist", path);

        if (checkUnsavedChanges && !EnsureUnsavedChanges()) return;

        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\tOpening scene at {}", fs::relative(path, data.project.GetAssetsPath()));
        else
            LogStatus("Opening scene at {}", fs::relative(path, data.project.GetAssetsPath()));

        data.scenePath = path;

        data.scene->Deinitialize();
        SceneSerializer::Deserialize(data.scene, path);
        data.scene->Initialize(); 

        SceneHierarchy::SetScene(data.scene);
        Properties::ClearSelectedData();

        ClearChanges();

        data.project.SetLastOpenedScenePath(fs::relative(path, data.project.GetAssetsPath()));

        data.title = "Copper Editor - " + data.project.name + ": " + data.project.GetLastOpenedSceneName();
        data.window.SetTitle(data.title); 

    }
    void OpenScene(bool checkUnsavedChanges) {

        CUP_FUNCTION();

        fs::path result = Utilities::OpenDialog("Open Scene", { "Copper Scene Files (.copper)", "*.copper" }, data.project.GetAssetsPath());

        if (result.empty()) {

            LogWarn("The Path Specified is empty or is not a Copper Scene File\n {}", result);
            return;

        }

        fs::path relativeToProjectAssets = fs::relative(result, data.project.GetAssetsPath());
        if (relativeToProjectAssets.empty()) {

            Input::ErrorPopup("Invalid Scene Path", "The scene you have tried to Open is outside of the Assets folder of this Project.");
            return;

        }

        OpenScene(result, checkUnsavedChanges);
        
    }
    void SaveScene() {

        CUP_FUNCTION();

        if (!UnsavedChanges()) return;

        LogStatus("Saving scene '{}'.", data.scenePath);

        if (data.scenePath.empty()) {

            SaveSceneAs();
            return;

        }

        SceneSerializer::Serialize(data.scene, data.scenePath);
        ProjectAssetDatabase::Save();

        data.project.SetLastOpenedScenePath(fs::relative(data.scenePath, data.project.GetAssetsPath()));

        ResetUnsavedChanges();

        data.title = "Copper Editor - " + data.project.name + ": " + data.project.GetLastOpenedSceneName();
        data.window.SetTitle(data.title);

        SaveEditorData();

    }
    void SaveSceneAs() {

        CUP_FUNCTION();

        fs::path path = Utilities::SaveDialog("Save Scene As", { "Copper Scene Files (.copper)", "*.copper" }, data.project.GetAssetsPath());
        if (path.empty()) return;

        fs::path relative = fs::relative(path, data.project.GetAssetsPath());
        if (relative.empty()) {

            Input::ErrorPopup("Invalid Scene", "The Place you want to save this scene is outside of this Project or starts with '..'");
            return;

        }

        data.scenePath = path;
        SaveScene();

    }

    void OpenSceneNext(const Copper::fs::path &path) {

        CUP_FUNCTION();

        data.nextScenePath = path;

    }
    void OpenSceneNext() {

        CUP_FUNCTION();

        fs::path result = Utilities::OpenDialog("Open Scene", { "Copper Scene Files (.copper)", "*.copper" }, data.project.GetAssetsPath());

            if (result.empty()) {

            LogWarn("The Path Specified is empty or is not a Copper Scene File\n {}", result);
            return;

        }

        fs::path relativeToProjectAssets = fs::relative(result, data.project.GetAssetsPath());
        if (relativeToProjectAssets.empty()) {

            Input::ErrorPopup("Invalid Scene Path", "The scene you have tried to Open is outside of the Assets folder of this Project.");
            return;

        }

        OpenSceneNext(result);

    }

    bool EnsureUnsavedChanges() {

        if (!UnsavedChanges()) return true;

        switch(Input::WarningPopup("Unsaved Changes", "There are unsaved changes made to this scene, do you wish to save before continuing ?")) {

            case Input::PopupResult::Yes: SaveScene();
            case Input::PopupResult::No: return true;
            case Input::PopupResult::Cancel: return false;
            default: return false;

        }

    }

    bool OnKeyPressed(const Event& e) {

        CUP_FUNCTION();

        bool control = Input::GetKeyState(KeyCode::LeftControl) == KeyState::Down || Input::GetKeyState(KeyCode::RightControl) == KeyState::Down;
        bool shift = Input::GetKeyState(KeyCode::LeftShift) == KeyState::Down || Input::GetKeyState(KeyCode::RightShift) == KeyState::Down;
        bool alt = Input::GetKeyState(KeyCode::LeftAlt) == KeyState::Down || Input::GetKeyState(KeyCode::RightAlt) == KeyState::Down;

        bool rightClick = Input::GetKeyState(KeyCode::Mouse1) == KeyState::Down;

        KeyEvent event = *(KeyEvent*) &e;

        switch (event.key) {

            case KeyCode::S: {

                if (data.state == EditorState::Play) break;
                if (!control) break;

                if (shift)
                    data.project.Save();
                else if (alt)
                    SaveSceneAs();
                else
                    SaveScene();

                SaveEditorData();

                break;

            }
            case KeyCode::B: {

                if (data.state == EditorState::Play || !control) break;
                data.project.Build();

                break;

            }
            case KeyCode::Q: {

                if (!data.viewport.IsFocused() || data.state == EditorState::Play || rightClick) break;

                data.viewport.SetGizmoOperation(Viewport::Gizmo::Translate);

                break;

            }
            case KeyCode::W: {

                if (!data.viewport.IsFocused() || data.state == EditorState::Play || rightClick) break;

                data.viewport.SetGizmoOperation(Viewport::Gizmo::Rotate);

                break;

            }
            case KeyCode::E: {

                if (!data.viewport.IsFocused() || data.state == EditorState::Play || rightClick) break;

                data.viewport.SetGizmoOperation(Viewport::Gizmo::Scale);

                break;

            }
            case KeyCode::C: {

                if (control && Properties::GetSelectedData().type == SelectedData::Type::Entity && (data.sceneHierarchy.IsFocused() || data.viewport.IsFocused()))
                    CopyToClipboard(Properties::GetSelectedData().entity);

                break;

            }
            case KeyCode::V: {

                if (control && !ClipboardEmpty() && (data.sceneHierarchy.IsFocused() || data.viewport.IsFocused())) {

                    Properties::SetSelectedEntity(PasteFromClipboard());
                    SetChanges();

                }

                break;

            }
            case KeyCode::Z: {
                
                if (control) {

                    if (shift)
                        RedoChange();
                    else
                        UndoChange();

                }

                break;

            }
            case KeyCode::F1: {

                if (data.state == EditorState::Edit || !shift) break;

                data.camView.SetIsAcceptingInput(false);

                Input::SetCursorLocked(false);
                Input::SetCursorVisible(true);

                break;

            }
            default: break;

        }

        return true;

    }
    bool OnWindowClose(const Event& e) {

        CUP_FUNCTION();

        if (!UnsavedChanges()) return true;

        LogStatus("\tUnsaved changes detected.");

        switch (Input::WarningPopup("Unsaved Changes", "There are Unsaved Changes in the project, do you wish to save the Project before exiting ?")) {

            case Input::PopupResult::Yes: {

                SaveScene();
                SaveEditorData();

                return true;

            }

            case Input::PopupResult::No: return true;
            case Input::PopupResult::Cancel: return false;
            default: return false;

        }

    }
    bool OnWindowFocused(const Event& e) {

        CUP_FUNCTION();

        const WindowFocusedEvent& event = static_cast<const WindowFocusedEvent&>(e);
        if (!event.focused || !data.project.ShouldRebuild()) return true;

        data.project.Build();

        return true;

    }

    bool IsInteractionBlocked() {

        if (data.interactionBlocked) return true;

        ImGuiContext* g = ImGui::GetCurrentContext();
        for (int32 i = 0; i < g->OpenPopupStack.Size; i++) {

            const ImGuiPopupData& popup = g->OpenPopupStack[i];
            if (popup.Window != nullptr && (popup.Window->Flags & ImGuiWindowFlags_Modal)) return true;

        }

        return false;

    }
    void SetIsInteractionBlocked(bool value) { data.interactionBlocked = value; }

    const std::string& GetWindowTitle() { return data.title; }
    void SetWindowTitle(const std::string& value) {

        CUP_FUNCTION();

        if (data.title == value) return;

        data.title = value;
        data.window.SetTitle(data.title);

    }
    
    Project& GetProject() { return data.project; }
    EditorState GetEditorState() { return data.state; }

    SceneCamera& GetSceneCam() { return data.viewport.GetSceneCamera(); }

    bool IsRuntimeRunning() { return data.state == EditorState::Play; }

}

#pragma region EntryPoint
#ifdef CU_CMAKE
#include <Config.h>
#endif
#include <Engine/Core/Entry.h>

void AppEntryPoint() {

    CUP_FUNCTION();

    // In the editor case, we have our own window that is bigger then the engine region
    // so we have to create and store it ourselves
    Window::InitializeBackend();
    Editor::data.window.Create("Copper Editor", 1280, 720);

    GetPostInitEvent() += Editor::Initialize;

    GetUpdateEvent() += Editor::Update;
    GetUIUpdateEvent() += Editor::UIUpdate;

    GetPreShutdownEvent() += Editor::OnWindowClose;
    GetPostShutdownEvent() += Editor::Shutdown;

}
#pragma endregion 

Window* GetEditorWindow() { return &Editor::data.window; }

bool IsGameAcceptingInput() { return Editor::data.camView.IsAcceptingInput(); }
