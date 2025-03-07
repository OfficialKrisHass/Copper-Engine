#include "EditorApp.h"

#include "Engine/Core/Args.h"
#include "Engine/Core/Core.h"

#include "Engine/Core/Log.h"
#include "Engine/Utilities/Math.h"

#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Physics/Raycast.h"

#include "Engine/UI/ImGui.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include "Core/SceneMeta.h"
#include "Core/FileWatcher.h"
#include "Core/SceneSerializer.h"

#include "Core/Utils/ModelLoader.h"

#include "Assets/ProjectAssetDatabase.h"

#include "Projects/Project.h"
#include "Projects/ProjectTemplate.h"

#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"
#include "Panels/Console.h"
#include "Panels/ProjectSettings.h"
#include "Panels/Profiler.h"
#include "Panels/ThemeEditor.h"

#include "Viewport/SceneCamera.h"

#include <GLM/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

#include <yaml-cpp/yaml.h>

#include <CMath/CMath.h>

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

        bool gameAcceptingInput = false;

        // Project

        Project project;
        bool scriptChanges = false;

        // Scene

        Scene* scene = nullptr;
        SceneMeta sceneMeta;
        fs::path scenePath;
        fs::path nextScenePath;

        bool changes = false;
        
        // Viewport

        UVector2I viewportSize = UVector2I(1280, 720);
        UVector2I viewportCentre;
        FrameBuffer viewportFBO;
        bool canLookViewport = true;

        SceneCamera sceneCam = SceneCamera(viewportSize);

        // Game Panel

        UVector2I gamePanelSize = UVector2I(1280, 720);
        bool gamePanelFocused = false;

        // Icons

        Texture playIcon;
        Texture stopIcon;
        
        // Panels

        SceneHierarchy sceneHierarchy;
        Properties properties;
        FileBrowser fileBrowser;
        Console console;
        ThemeEditor themeEditor;

        // Selected assets

        Entity selectedEntity;
        fs::path selectedFile = "";

        // Theme Editor

        bool themeEditorOpen = false;

        // Debug only
#ifdef CU_DEBUG
        bool imguiDemoWindow = false;
#endif

    };

    EditorData data;

    void LoadEditorData();
    void SaveEditorData();

    void RenderDockspace();
    void RenderGamePanel();
    void RenderViewport();
    void RenderToolbar();
    void RenderMenu();

    void NewProject();

    void FileChangedCallback(const fs::path& path, const FileWatcher::FileChangeType changeType);
    void CopyScriptingAPI();

    void StartEditorRuntime();
    void StopEditorRuntime();

    bool OnKeyPressed(const Event& e);
    bool OnWindowClose(const Event& e);

    void Initialize() {

        CUP_FUNCTION();

        GetWindow().AddKeyPressedEventFunc(Editor::OnKeyPressed);

        MainUIContext().LoadFont(ExecutableFolder() / "assets/Fonts/open-sans.regular.ttf");

        data.scene = GetScene();

        data.viewportFBO = FrameBuffer(data.viewportSize);
        
        data.playIcon.Create(ExecutableFolder() / "assets/Icons/PlayButton.png", Texture::Format::RGBA);
        data.stopIcon.Create(ExecutableFolder() / "assets/Icons/StopButton.png", Texture::Format::RGBA);

        data.fileBrowser.Initialize();

        FileWatcher::AddCallback(FileChangedCallback);

        LoadEditorData();

        data.themeEditor.LoadTheme(ExecutableFolder() / "assets/Themes/Default.cutheme");

#ifdef CU_LINUX
        data.project.RunPremake();
#endif

    }
    void Shutdown() {

        CUP_FUNCTION();

        SaveEditorData();

    }

    void SaveEditorData() {

        CUP_FUNCTION();

        YAML::Emitter out;

        out << YAML::BeginMap; //Start

        out << YAML::Key << "Last Project" << YAML::Value << data.project.GetPath();

        out << YAML::EndMap; //End

        std::ofstream file(ExecutableFolder() / "assets/EditorData.cu");
        file << out.c_str();

        data.project.Save();

    }
    void LoadEditorData() {

        CUP_FUNCTION();

        if (!fs::exists(ExecutableFolder() / "assets/EditorData.cu")) {

            LogWarn("EditorData.cu is missing, generating a default one");
            SaveEditorData();

        }

        YAML::Node main;
        try { main = YAML::LoadFile((ExecutableFolder() / "assets/EditorData.cu").string()); }
        catch (YAML::Exception e) {

            Input::ErrorPopup("EditorData read failed", "Could not read the EditorData.cu file.\n\nIt shuld be located here:\n" + (ExecutableFolder() / "assets/EditorData.cu").string() + "\n\nError message:\n" + e.what());
            exit(1);

        }
    
        if (!Args::ProjectPath().empty()) {

            data.project.Open(Args::Get(0));
            return;

        }

        std::string path = main["Last Project"].as<std::string>();
        if (!std::filesystem::exists(path)) {

            switch (Input::WarningPopup("Last opened project doesn't exist", "The last opened project no longer exists, do you wish to open a project manually, or exit the Editor ?\n\nLast opened project path:\n" + path)) {

                case Input::PopupResult::Yes: data.project.Open(); return;
                case Input::PopupResult::No: exit(1);
                default: exit(1);

            }

        }

        data.project.Open(path);

    }

    void Update() {

        CUP_FUNCTION();
        CUP_START_FRAME("Editor");

        FileWatcher::PollChanges();

        if (!data.nextScenePath.empty()) {

            OpenScene(data.nextScenePath);
            data.nextScenePath.clear();

        }

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
        if (data.themeEditorOpen) data.themeEditor.UIRender();
        RenderGamePanel();
        RenderViewport();

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

    void RenderDockspace() {

        CUP_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen) {

            ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags); //Dockspace
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();

        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        }

        style.WindowMinSize.x = minWinSizeX;

    }
    void RenderGamePanel() {

        CUP_FUNCTION();
        CUP_START_FRAME("Game Panel");

        //Imgui::Begin returns a bool based on if the Window is visible/open
        //So, we store that and then Render the scene and window Only if it's visible
        //to save some performance
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
        bool open = ImGui::Begin("Game");

        if (!open) {

            ImGui::End();
            ImGui::PopStyleVar();

            CUP_END_FRAME();

            return;

        }
        if (!data.project || !data.scene->GetMainCamera()) {

            ImGui::Text("No Camera Available!");

            ImGui::End();
            ImGui::PopStyleVar();

            CUP_END_FRAME();

            return;

        }

        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        data.gamePanelSize = UVector2I((uint32) windowSize.x, (uint32) windowSize.y);

        SetWindowSize(data.gamePanelSize);

        ImGui::Image(reinterpret_cast<void*>((uint64) GetMainFBO().GetColorTextureID()), windowSize, ImVec2 {0, 1}, ImVec2 {1, 0});

        if (data.state == EditorState::Play && !data.gameAcceptingInput && ImGui::IsItemClicked()) {

            data.gameAcceptingInput = true;

            Input::SetCursorPosition((float) data.viewportCentre.x, (float) data.viewportCentre.y);
            Input::SetCursorLocked(true);
            Input::SetCursorVisible(false);

        }

        ImGui::End();
        ImGui::PopStyleVar();

        CUP_END_FRAME();

    }
    void RenderViewport() {

        CUP_FUNCTION();
        CUP_START_FRAME("Viewport");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        bool open = ImGui::Begin("Viewport");
        if (!open) {

            ImGui::End();
            ImGui::PopStyleVar();

            CUP_END_FRAME();

            return;

        }

        //TODO: Either Change ImGui To use UVector2I or edit Copper Code to use ImVec2
        //      so that we don't have to allocate memory for the UVector2I
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 windowPos = ImGui::GetWindowPos();

        data.viewportSize = UVector2I((uint32) windowSize.x, (uint32) windowSize.y);

        data.viewportCentre = data.viewportSize / 2;
        data.viewportCentre.x += (uint32) windowPos.x;
        data.viewportCentre.y += (uint32) windowPos.y;

        if (data.viewportFBO.GetWidth() != data.viewportSize.x || data.viewportFBO.GetHeight() != data.viewportSize.y) {

            //We don't need to Call SetWindowSize because if the Viewport size is changed
            //it only affects the Viewport, not the Actualy Game Engine and the Main Game Panel
            data.viewportFBO.Resize(data.viewportSize);
            data.sceneCam.Resize(data.viewportSize);

        }

        //We need to Clear the Color because if we don't we just get a black image
        data.viewportFBO.Bind();
        RendererAPI::ClearColor(Color(0.18f, 0.18f, 0.18f));

        data.sceneCam.Update();
        if (data.scene)
            data.scene->Render(&data.sceneCam);

        //After we are done rendering we are safe to unbind the FBO unless we want to modify it any way
        data.viewportFBO.Unbind();

        ImGui::Image(reinterpret_cast<void*>((uint64) data.viewportFBO.GetColorTextureID()), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        //Gizmos that I stol... I mean, taken inspiration from The Chernos Game Engine series
        //Yeah, I definitely didn't copy this entire chunk of code that I don't understand but
        //magically works, naaah.
        InternalEntity* selectedObj = data.selectedEntity;
        if (selectedObj) {

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float wWidth = (float) ImGui::GetWindowWidth();
            float wHeight = (float) ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, wWidth, wHeight);

            Matrix4 camProjection = data.sceneCam.CreateProjectionMatrix();
            Matrix4 camView = data.sceneCam.CreateViewMatrix();
            glm::mat4 transform = selectedObj->GetTransform()->TransformMatrix();

            // Snapping
            bool snap = Input::IsKey(KeyCode::LeftControl);
            float snapValue = 0.5f;
            if (data.project.GetGizmoType() == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;

            float snapValues[3] = {snapValue, snapValue, snapValue};

            ImGuizmo::Manipulate(&(camView.cols[0].x), &(camProjection.cols[0].x),
                                 (ImGuizmo::OPERATION) data.project.GetGizmoType(), ImGuizmo::LOCAL, glm::value_ptr(transform),
                                 nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing()) {

                glm::vec3 position, rotation, scale;

                Math::DecomposeTransform(transform, position, rotation, scale);

                //glm::vec3 deltaRotation = (Vector3) rotation - selectedObj->GetTransform()->rotation;
                selectedObj->GetTransform()->SetPosition(position);
                //selectedObj->GetTransform()->rotation += deltaRotation;
                selectedObj->GetTransform()->SetScale(scale);

                //The rotation doesn't work for some reason, it keeps wiggling around
                //Unfortunately I'm dum dum so this is what you get :) uwu

                //TODO: Dont be dum dum like a friggin idiot and learn how to do it owo

            }

        }

        data.canLookViewport = ImGui::IsItemHovered();
        data.sceneCam.SetCanLook(data.canLookViewport);
        
        ImGui::End();
        ImGui::PopStyleVar();

        CUP_END_FRAME();

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

            if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64) data.playIcon.GetID()), buttonSize, {0, 1}, {1, 0}) && data.project)
                StartEditorRuntime();

        } else if (data.state == EditorState::Play) {

            if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64) data.stopIcon.GetID()), buttonSize, {0, 1}, {1, 0}) && data.project)
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
                    NewProject();
                if (ImGui::MenuItem("Open Project"))
                    data.project.Open();
                if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S", false, data.project)) {

                    SaveScene();

                    data.project.Save();
                    SaveEditorData();

                }

                ImGui::Separator();

                if (ImGui::MenuItem("Create Template", 0, false, data.project))
                    CreateTemplateFromProject(data.project);

                ImGui::Separator();

                if (ImGui::MenuItem("Build Scripts", "Ctrl+B", false, data.project))
                    data.project.BuildScripts();

                ImGui::Separator();

                if (ImGui::MenuItem("Project Settings"))
                    ProjectSettings::Open();
                
                ImGui::Separator();

                if (ImGui::MenuItem("Copy Copper Scripting API", 0, false, data.project))
                    CopyScriptingAPI();

                ImGui::EndMenu();

            }

            if(ImGui::BeginMenu("File")) {

                if(ImGui::MenuItem("New Scene", 0, false, data.project))
                    NewScene();
                if(ImGui::MenuItem("Open Scene", 0, false, data.project))
                    OpenSceneNext();
                if(ImGui::MenuItem("Save Scene", "Ctr+S", false, data.project))
                    SaveScene();
                if(ImGui::MenuItem("Save Ass", "Ctrl+Alt+S", false, data.project))
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

                if (ImGui::DragFloat("Speed", &data.sceneCam.speed, 0.01f, 0.001f, 50.0f, "%.4f"))
                    SetChanges(true);
                if (ImGui::DragFloat("Sensitivity", &data.sceneCam.sensitivity, 0.1f, 1.0f, 1000.0f))
                    SetChanges(true);

                ImGui::EndMenu();

            }

            if (ImGui::BeginMenu("Debug")) {

                if (ImGui::MenuItem("Profiler"))
                    Profiler::Open();

#ifdef CU_DEBUG
                ImGui::Separator();

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

    }
    void StopEditorRuntime() {

        CUP_FUNCTION();

        data.state = EditorState::Edit;

        data.scene->Cleanup();
        SceneSerializer::Deserialize(data.scene, ExecutableFolder() / "assets/Temp/scene_lock.copper");
        data.scene->Initialize();

        data.gameAcceptingInput = false;

        Input::SetCursorLocked(false);
        Input::SetCursorVisible(true);

    }

    void FileChangedCallback(const fs::path& path, const FileWatcher::FileChangeType changeType) {

        CUP_FUNCTION();

        if (path.extension().string() != ".cs") return;

#ifdef CU_LINUX
        if (changeType != FileWatcher::FileChangeType::Changed)
            data.project.RunPremake();
#endif
        data.project.BuildScripts();

    }

    void NewProject() {

        CUP_FUNCTION();

        fs::path path = Utilities::FolderOpenDialog("New Project", data.project ? data.project.GetPath().parent_path() : ROOT_DIR);
        if (path.empty()) return;

        // Create the Project

        data.project = Project(path);

        FileBrowser::SetRelativeDir("");

#ifdef CU_LINUX
        data.project.RunPremake();
#endif
        data.project.BuildScripts();

        NewScene();

        data.changes = false;
        data.title = "Copper Editor - " + data.project.GetName() + ":";
        Input::SetWindowTitle(data.title);

    }

    void CopyScriptingAPI() {

        CUP_FUNCTION();

        std::ifstream dllSrc(ExecutableFolder() / "assets/ScriptingAPI/Copper-ScriptingAPI.dll", std::ios::binary);
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
        
    }
    void OpenScene(const fs::path& path) {

        CUP_FUNCTION();

        if(data.changes) {

            switch(Input::WarningPopup("Unsaved Changes", "There are unsaved changes made to this scene, do you wish to save before opening a new scene ?")) {

                case Input::PopupResult::Yes: SaveScene(); break;
                case Input::PopupResult::No: break;
                case Input::PopupResult::Cancel: return;
                default: return;

            }

        }

        data.scenePath = path;

        data.scene->Cleanup();
        SceneSerializer::Deserialize(data.scene, path);
        data.scene->Initialize(); 

        data.sceneMeta.Deserialize(path);

        SceneHierarchy::SetScene(data.scene);

        data.changes = false;
        data.title = "Copper Editor - " + data.project.GetName() + ": " + data.scene->GetName();
        Input::SetWindowTitle(data.title);

        data.project.SetLastOpenedScenePath(fs::relative(path, data.project.GetAssetsPath()));

    }
    void OpenScene() {

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

        OpenScene(result);
        
    }
    void SaveScene() {

        CUP_FUNCTION();

        if (data.scenePath.empty()) {

            SaveSceneAs();
            return;

        }

        SceneSerializer::Serialize(data.scene, data.scenePath);
        data.sceneMeta.Serialize(data.scenePath);

        data.changes = false;
        data.title = "Copper Editor - TestProject: ";
        data.title += data.scene->GetName();
        Input::SetWindowTitle(data.title);

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

        SceneSerializer::Serialize(data.scene, path);
        data.sceneMeta.Serialize(path);

        data.project.SetLastOpenedScenePath(relative);

        data.changes = false;
        data.title = "Copper Editor - TestProject: ";
        data.title += data.scene->GetName();
        Input::SetWindowTitle(data.title);
        
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

    bool OnKeyPressed(const Event& e) {

        CUP_FUNCTION();

        bool control = Input::IsKey(KeyCode::LeftControl) || Input::IsKey(KeyCode::RightControl);
        bool shift = Input::IsKey(KeyCode::LeftShift) || Input::IsKey(KeyCode::RightShift);
        bool alt = Input::IsKey(KeyCode::LeftAlt) || Input::IsKey(KeyCode::RightAlt);

        bool rightClick = Input::IsButton(MouseCode::Button2);

        KeyEvent event = *(KeyEvent*) &e;

        switch (event.key) {

            case KeyCode::S: {

                if (data.state == EditorState::Play) break;
                if (control && shift) {

                    data.project.Save();
                    SaveEditorData();
                    SaveScene();

                }
                if (control && alt)
                    SaveSceneAs();
                if (control)
                    SaveScene();

                break;

            }
            case KeyCode::B: {

                if (data.state == EditorState::Play || !control) break;

                data.project.BuildScripts();

                break;

            }
            case KeyCode::Q: {

                if (data.state == EditorState::Play || rightClick) break;

                data.project.SetGizmoType(ImGuizmo::TRANSLATE);

                break;

            }
            case KeyCode::W: {

                if (data.state == EditorState::Play || rightClick) break;

                data.project.SetGizmoType(ImGuizmo::ROTATE);

                break;

            }
            case KeyCode::E: {

                if (data.state == EditorState::Play || rightClick) break;

                data.project.SetGizmoType(ImGuizmo::SCALE);

                break;

            }
            case KeyCode::F1: {

                if (data.state == EditorState::Edit || !shift) break;

                data.gameAcceptingInput = false;

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

        if (!data.changes) return true;

        switch (Input::WarningPopup("Unsaved Changes", "There are Unsaved Changes in the project, do you wish to save the Project before exiting ?")) {

            case Input::PopupResult::Yes: {

                SaveScene();
                SaveEditorData();

                return true;

            }

            case Input::PopupResult::No:
                return true;
            case Input::PopupResult::Cancel:
                return false;
            default:
                return false;

        }

    }
    
    const Project& GetProject() { return data.project; }
    SceneCamera& GetSceneCam() { return data.sceneCam; }

    SceneMeta* GetSceneMeta() { return &data.sceneMeta; }

    Entity& GetSelectedEntity() { return data.selectedEntity; }
    fs::path& GetSelectedFile() { return data.selectedFile; }

    void SetSelectedEntity(const Copper::Entity& value) {

        data.selectedEntity = value;
        data.selectedFile = "";

    }
    void SetSelectedFile(const Copper::fs::path& value) {

        data.selectedFile = value;
        data.selectedEntity = nullptr;

    }

    UVector2I GetViewportSize() { return data.viewportSize; }

    bool IsRuntimeRunning() { return data.state == EditorState::Play; }

    // TODO: This feature is not working, has not been working for the past year, isn't
    // even used in 90% of the places it should be, and also is done in the stupidest way imaginable.
    // Am I going to fix it in 0.3 ? ..... No
    void SetChanges(bool value) {

        CUP_FUNCTION();

        if (data.state == EditorState::Play) return;

        data.changes = value;

        data.title = "Copper Editor - " + data.project.GetName() + ": " + data.scene->GetName() + "*";
        Input::SetWindowTitle(data.title);

    }

}

#pragma region EntryPoint
#include <Engine/Core/Entry.h>

void AppEntryPoint() {

    CUP_FUNCTION();

    // In the editor case, we have our own window that is bigger then the engine region
    // so we have to create and store it ourselves
    Editor::data.window = Window("Copper Editor", 1280, 720);

    AddPostInitEventFunc(Editor::Initialize);
    AddUpdateEventFunc(Editor::Update);
    AddUIUpdateEventFunc(Editor::UIUpdate);
    AddPreShutdownEventFunc(Editor::OnWindowClose);
    AddPostShutdownEventFunc(Editor::Shutdown);

}
#pragma endregion 

Window* GetEditorWindow() { return &Editor::data.window; }
UVector2I GetViewportCentre() { return Editor::data.viewportCentre; }

bool IsGameAcceptingInput() { return Editor::data.gameAcceptingInput; }
