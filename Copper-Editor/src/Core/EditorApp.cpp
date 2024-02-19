#include "EditorApp.h"

#include "Engine/Core/Args.h"
#include "Engine/Core/Core.h"

#include "Engine/Utilities/Math.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/UI/ImGui.h"

#include "Core/SceneMeta.h"
#include "Core/ProjectFileWatcher.h"

#include "Core/Utils/ModelLoader.h"

#include "Projects/Project.h"
#include "Projects/ProjectTemplate.h"
#include "Projects/ProjectChecker.h"

#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"
#include "Panels/Console.h"
#include "Panels/ProjectSettings.h"
#include "Panels/ThemeEditor.h"

#include "Viewport/SceneCamera.h"

#include <GLM/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

#include <yaml-cpp/yaml.h>

#include <CMath/CMath.h>

using namespace Copper;

namespace Editor {

	struct EditorData {

		// Core Data

		EditorState state;
		Window window;
		std::string title;

		// Project

		Project project;
		bool scriptChanges;

		// Scene

		Scene* scene;
		MetaFile::SceneMeta sceneMeta;
		bool changes;
		
		// Viewport

		UVector2I viewportSize;
		UVector2I viewportCentre;
		FrameBuffer viewportFBO;
		bool canLookViewport;

		SceneCamera sceneCam;

		// Game Panel

		UVector2I gamePanelSize;
		bool gamePanelFocused;

		// Icons

		Texture playIcon;
		Texture stopIcon;
		
		// Panels

		SceneHierarchy sceneHierarchy;
		Properties properties;
		FileBrowser fileBrowser;
		Console console;
		ThemeEditor themeEditor;

		// Theme Editor
		bool themeEditorOpen = false;

		// Scripting

		bool wasCursorLocked = false;
		bool wasCursorVisible = true;

	};

	EditorData data;

	void Initialize();
	void UIUpdate();
	void Shutdown();

	void LoadEditorData();
	void SaveEditorData(bool saveProject = true);

	void RenderDockspace();
	void RenderGamePanel();
	void RenderViewport();
	void RenderToolbar();
	void RenderMenu();

	void NewProject();
	void OpenProject(const fs::path& path);
	void OpenProject();

	void FileChangedCallback(const fs::path& path, const ProjectFileWatcher::FileChangeType& changeType);
	void CopyScriptingAPI();

	void StartEditorRuntime();
	void StopEditorRuntime();

	bool OnKeyPressed(const Event& e);
	bool OnWindowClose(const Event& e);
	bool OnWindowFocused(const Event& e);

	bool OnEntityCreated(const Event& e);
	bool OnEntityRemoved(const Event& e);

#ifdef CU_LINUX
	void RunPremake();
#endif

	void Initialize() {

		CUP_FUNCTION();

		GetWindow().AddWindowFocusedEventFunc(Editor::OnWindowFocused);
		GetWindow().AddKeyPressedEventFunc(Editor::OnKeyPressed);

		AddEntityCreatedEventFunc(OnEntityCreated);
		AddEntityRemovedEventFunc(OnEntityRemoved);

		LoadMainUIContextFont("assets/Fonts/open-sans.regular.ttf");

		data.state = Edit;
		data.viewportSize = UVector2I(1280, 720);
		data.gamePanelSize = UVector2I(1280, 720);
		data.viewportFBO = FrameBuffer(data.viewportSize);
		
		data.playIcon.Create("assets/Icons/PlayButton.png", Texture::Format::RGBA);
		data.stopIcon.Create("assets/Icons/StopButton.png", Texture::Format::RGBA);
		
		data.sceneHierarchy = SceneHierarchy();
		data.properties = Properties();
		data.fileBrowser = FileBrowser("");
		data.console = Console();
		data.themeEditor = ThemeEditor();

		data.sceneCam = SceneCamera(data.viewportSize);

		data.properties.SetSelectedObject(data.sceneHierarchy.GetSelectedEntity());
		
		ProjectFileWatcher::AddFileChangeCallback(FileChangedCallback);

		LoadEditorData();

		data.themeEditor.LoadTheme("assets/Themes/Default.cutheme");

	#ifdef CU_LINUX
		RunPremake();
	#endif

	}
	void Shutdown() {

		SaveEditorData();

	}

	void SaveEditorData(bool saveProject) {

		YAML::Emitter out;

		out << YAML::BeginMap; //Start

		out << YAML::Key << "Last Project" << YAML::Value << data.project.path;

		out << YAML::EndMap; //End

		std::ofstream file("assets/EditorData.cu");
		file << out.c_str();

		if (saveProject && data.project) data.project.Save();

	}
	void LoadEditorData() {

		if (!fs::exists("assets/EditorData.cu")) {

			LogWarn("EditorData.cu is missing, generating a default one");

			data.project.path = "";
			SaveEditorData(false);

		}

		YAML::Node main;
		try { main = YAML::LoadFile("assets/EditorData.cu"); } catch (YAML::Exception e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}
		
		std::string path = main["Last Project"].as<std::string>();
		if (!std::filesystem::exists(path)) {

			OpenProject();
			return;

		}

		OpenProject(path);

	}

	void UIUpdate() {
		
		CUP_START_FRAME("Editor UI");
		
		RenderDockspace();
		RenderToolbar();
		RenderMenu();
		
		data.console.UIRender();
		data.fileBrowser.UIRender();
		data.properties.UIRender();
		data.sceneHierarchy.UIRender();
		if (data.themeEditorOpen) data.themeEditor.UIRender();
		RenderGamePanel();
		RenderViewport();

		ProjectSettings::UIRender();

		ImGui::End(); //Dockspace

		CUP_END_FRAME();

	}

	void RenderDockspace() {

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

		if (opt_fullscreen) ImGui::PopStyleVar(2);

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

		CUP_START_FRAME("Game Panel");

		//Imgui::Begin returns a bool based on if the Window is visible/open
		//So, we store that and then Render the scene and window Only if it's visible
		//to save some performance
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
		bool open = ImGui::Begin("Game");

		if (!open) {

			ImGui::End();
			ImGui::PopStyleVar();

			return;

		}
		if (!data.project || !data.scene->cam) {

			ImGui::Text("No Camera Available!");

			ImGui::End();
			ImGui::PopStyleVar();

			return;

		}

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		data.gamePanelSize = UVector2I((uint32) windowSize.x, (uint32) windowSize.y);

		SetWindowSize(data.gamePanelSize);

		ImGui::Image(reinterpret_cast<void*>((uint64) GetMainFBOTexture()), windowSize, ImVec2 {0, 1}, ImVec2 {1, 0});

		if (ImGui::IsItemClicked() && !AcceptInputDuringRuntime() && IsSceneRuntimeRunning()) {
			
			Input::SetCursorLocked(data.wasCursorLocked);
			Input::SetCursorVisible(data.wasCursorVisible);
			SetAcceptInputDuringRuntime(true);
		
		}

		ImGui::End();
		ImGui::PopStyleVar();

		CUP_END_FRAME();

	}
	void RenderViewport() {

		CUP_START_FRAME("Viewport");

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		bool open = ImGui::Begin("Viewport");
		if (!open) {

			ImGui::End();
			ImGui::PopStyleVar();

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

		if (data.viewportFBO.Width() != data.viewportSize.x || data.viewportFBO.Height() != data.viewportSize.y) {

			//We don't need to Call SetWindowSize because if the Viewport size is changed
			//it only affects the Viewport, not the Actualy Game Engine and the Main Game Panel
			data.viewportFBO.Resize(data.viewportSize);
			data.sceneCam.Resize(data.viewportSize);

		}

		//We need to Clear the Color because if we don't we just get a black image
		data.viewportFBO.Bind();
		RendererAPI::ClearColor(Color(0.18f, 0.18f, 0.18f));

		data.sceneCam.Update();
		if (data.scene) data.scene->Render(&data.sceneCam);

		//After we are done rendering we are safe to unbind the FBO unless we want to modify it any way
		data.viewportFBO.Unbind();

		ImGui::Image(reinterpret_cast<void*>((uint64) data.viewportFBO.ColorTextureID()), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		//Gizmos that I stol... I mean, taken inspiration from The Chernos Game Engine series
		//Yeah, I definitely didn't copy this entire chunk of code that I don't understand but
		//magically works, naaah.
		InternalEntity* selectedObj = *(data.sceneHierarchy.GetSelectedEntity());
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
			if (data.project.gizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;

			float snapValues[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(&(camView.cols[0].x), &(camProjection.cols[0].x),
								 (ImGuizmo::OPERATION) data.project.gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
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

		if (data.state == Edit) {

			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64) data.playIcon.ID()), buttonSize, {0, 1}, {1, 0}) && data.project) StartEditorRuntime();

		} else if (data.state == Play) {

			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64) data.stopIcon.ID()), buttonSize, {0, 1}, {1, 0}) && data.project) StopEditorRuntime();

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

		if(ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("Project")) {

				if (ImGui::MenuItem("New Project")) NewProject();
				if (ImGui::MenuItem("Open Project")) OpenProject();
				if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S", false, data.project)) { data.project.Save(); SaveEditorData(); SaveScene(); }

				ImGui::Separator();

				if (ImGui::MenuItem("Create Template", 0, false, data.project)) CreateTemplateFromProject(data.project);

				ImGui::Separator();

				if (ImGui::MenuItem("Build Solution", "Ctrl+B", false, data.project)) data.project.BuildSolution();

				ImGui::Separator();

				if (ImGui::MenuItem("Project Settings")) ProjectSettings::Open();
				
				ImGui::Separator();

				if (ImGui::MenuItem("Copy Copper Scripting API", 0, false, data.project)) CopyScriptingAPI();

				ImGui::EndMenu();

			}

			if(ImGui::BeginMenu("File")) {

				if(ImGui::MenuItem("New Scene", 0, false, data.project))				NewScene();
				if(ImGui::MenuItem("Open Scene", 0, false, data.project))				OpenScene();
				if(ImGui::MenuItem("Save Scene", "Ctr+S", false, data.project))		SaveScene();
				if(ImGui::MenuItem("Save Ass", "Ctrl+Alt+S", false, data.project))	SaveSceneAs();

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

				if (ImGui::DragFloat("Speed", &data.sceneCam.speed, 0.01f, 0.001f, 50.0f, "%.4f")) SetChanges(true);
				if (ImGui::DragFloat("Sensitivity", &data.sceneCam.sensitivity, 0.1f, 1.0f, 1000.0f)) SetChanges(true);

				ImGui::EndMenu();
				
			}

			ImGui::EndMenuBar();
			
		}
		
	}

	void StartEditorRuntime() {

		data.state = Play;

		SaveScene();
		data.scene->StartRuntime();

		SetAcceptInputDuringRuntime(true);

	}
	void StopEditorRuntime() {

		if (data.wasCursorLocked)
			Input::SetCursorLocked(false);

		data.state = Edit;
		fs::path savedPath = data.scene->path;
		Entity savedSelectedEntity = *data.sceneHierarchy.GetSelectedEntity();

		data.scene->StopRuntime();
		data.scene->Deserialize(savedPath);
		data.sceneMeta.Deserialize(data.scene);

		data.sceneHierarchy.SetSelectedEntity(savedSelectedEntity);

		Input::SetCursorLocked(false);
		Input::SetCursorVisible(true);
		SetAcceptInputDuringRuntime(false);

	}

	void FileChangedCallback(const fs::path& path, const ProjectFileWatcher::FileChangeType& changeType) {

		data.scriptChanges = true;

	#ifdef CU_LINUX
		if (changeType == ProjectFileWatcher::FileChangeType::Created || changeType == ProjectFileWatcher::FileChangeType::Deleted)
			RunPremake();
	#endif

	}

	void NewProject() {

		//Open the Folder Dialog
		//TODO : Either make our own Folder Open Dialog or Start using the Windows new System
		//TODO #2 : Fixed... well for linux
		fs::path path = Utilities::FolderOpenDialog("New Project", data.project ? data.project.path.parent_path() : ROOT_DIR);
		if (path.empty()) { LogWarn("Path is Invalid or Empty"); return; }

		//Create the Project
		data.project = Project(path.filename().string(), path);
		FileBrowser::SetRelativeDir("");

		CreateProjectFromTemplate("assets/Templates/LinuxTesting", data.project);

	#ifdef CU_LINUX
		RunPremake();
	#endif
		
		data.project.BuildSolution(true);

		OpenScene(data.project.assetsPath / data.project.lastOpenedScene);

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": EmptyTemplate";
		Input::SetWindowTitle(data.title);

	}
	void OpenProject(const fs::path& path) {
		
		try { data.project.Load(path); }
		catch (YAML::Exception e) { LogError("Something went wrong trying to open the Project file.\n    {}", e.what()); }

		data.scene = GetScene();
		FileBrowser::SetRelativeDir("");

		if (uint16_t issueFlags = ProjectChecker::CheckProject(data.project)) {

			if (Input::WarningPopup("Corrupted Project", "This project is missing some of the core folders and/or files that are required by the Editor to function properly, would you like to attempt to fix the Project ?") == Input::PopupResult::Yes)
				ProjectChecker::FixProject(data.project, issueFlags);

		}

		ProjectFileWatcher::Stop();
		ProjectFileWatcher::SetDirectory(data.project.assetsPath);
		ProjectFileWatcher::Start();

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": ";
		Input::SetWindowTitle(data.title);

		// In a completely new project, or a cleaned project, this should fail as the Assembly does not exist
		// so it will build the project and attempt to load it again, if that fails, it tries again 9 more times
		// and then exit the application

		bool reloadSuccess = Scripting::Load(data.project.path / "Binaries" / (data.project.name + ".dll"));
		uint32 i = 0;
		while (!reloadSuccess && i < 10) {

			LogError("Failed to Load the Project Assembly, attempt #{}", i);

			reloadSuccess = data.project.BuildSolution();
			i++;

		}
		if (!reloadSuccess) {

			Input::ErrorPopup("Build failed", "Failed to build the project multiple times, Look into the log to see more information. Exiting application now");
			exit(-1);

		}

		OpenScene(data.project.lastOpenedScene);

	}
	void OpenProject() {

		fs::path path = Utilities::FolderOpenDialog("Open Project", data.project ? data.project.path.parent_path() : ROOT_DIR);
		if (path.empty()) { LogWarn("path is Invalid or empty"); return; }

		OpenProject(path);

	}

	void CopyScriptingAPI() {

		std::ifstream dllSrc("assets/ScriptAPI/Copper-ScriptingAPI.dll", std::ios::binary);
		std::fstream dllDst;

		dllDst.open(data.project.path / "Binaries/Copper-ScriptingAPI.dll", std::ios::out | std::ios::binary);
		dllDst << dllSrc.rdbuf();
		dllDst.close();

	}

	void NewScene() {

		*data.scene = Scene();

		data.sceneHierarchy.SetScene(data.scene);
		
	}
	void OpenScene(const fs::path& path) {

		if(data.changes) {

			switch(Input::WarningPopup("Unsaved Changes", "There are unsaved changes made to this scene, do you wish to save before opening a new scene ?")) {

			case Input::PopupResult::Yes: SaveScene(); break;
			case Input::PopupResult::No: break;
			case Input::PopupResult::Cancel: return;
				
			}
			
		}

		data.scene->Deserialize(path);
		data.sceneMeta.Deserialize(data.scene);

		data.sceneHierarchy.SetScene(data.scene);

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": ";
		data.title += data.scene->name;
		Input::SetWindowTitle(data.title);

		data.project.lastOpenedScene = fs::relative(path, data.project.assetsPath);
		
	}
	void OpenScene() {

		fs::path result = Utilities::OpenDialog("Open Scene", { "Copper Scene Files (.copper)", "*.copper" }, data.project.assetsPath);

		if(result.empty()) { LogWarn("The Path Specified is empty or is not a Copper Scene File\n {}", result); return; }

		fs::path relativeToProjectAssets = fs::relative(result, data.project.assetsPath);
		if (relativeToProjectAssets.empty()) {

			Input::ErrorPopup("Invalid Scene Path", "The scene you have tried to Open is outside of the Assets folder of this Project.");
			return;

		}

		OpenScene(result);
		
	}
	void SaveScene() {
		
		if(!data.scene->path.empty()) {

			data.scene->Serialize(data.scene->path);
			data.sceneMeta.Serialize();

			data.changes = false;
			data.title = "Copper Editor - TestProject: ";
			data.title += data.scene->name;
			Input::SetWindowTitle(data.title);
			
			return;
			
		}

		SaveSceneAs();
		
	}
	void SaveSceneAs() {

		fs::path path = Utilities::SaveDialog("Save Scene As", { "Copper Scene Files (.copper)", "*.copper" }, data.project.assetsPath);

		if (path.empty()) return;

		fs::path relativeToProjectAssets = fs::relative(path, data.project.assetsPath);
		if (relativeToProjectAssets.empty()) {

			Input::ErrorPopup("Invalid Scene", "The Place you want to save this scene is outside of this Project or starts with '..'");
			return;

		}

		data.scene->Serialize(path);
		data.sceneMeta.Serialize();

		data.changes = false;
		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene->name;
		Input::SetWindowTitle(data.title);
		
	}

	bool OnKeyPressed(const Event& e) {

		/*if (data.state == Play) return true;*/

		bool control = Input::IsKey(KeyCode::LeftControl) || Input::IsKey(KeyCode::RightControl);
		bool shift = Input::IsKey(KeyCode::LeftShift) || Input::IsKey(KeyCode::RightShift);
		bool alt = Input::IsKey(KeyCode::LeftAlt) || Input::IsKey(KeyCode::RightAlt);

		bool rightClick = Input::IsButton(MouseCode::Button2);

		KeyEvent event = *(KeyEvent*) &e;

		switch (event.key) {

			case KeyCode::S: {

				if (data.state == Play) break;

				if (control && shift) { data.project.Save(); SaveEditorData(); SaveScene(); }
				if (control && alt) SaveSceneAs();
				if (control) SaveScene();

				break;

			}
			case KeyCode::B: {

				if (data.state == Play) break;

				if (control) data.project.BuildSolution();

				break;

			}
			case KeyCode::Q: {

				if (data.state == Play) break;
				if (!rightClick) data.project.gizmoType = ImGuizmo::TRANSLATE;

				break;

			}
			case KeyCode::W: {

				if (data.state == Play) break;
				if (!rightClick) data.project.gizmoType = ImGuizmo::ROTATE;

				break;

			}
			case KeyCode::E: {

				if (data.state == Play) break;
				if (!rightClick) data.project.gizmoType = ImGuizmo::SCALE;

				break;

			}
			case KeyCode::Escape: {

				data.wasCursorLocked = Input::IsCursorLocked();
				data.wasCursorVisible = Input::IsCursorVisible();

				Input::SetCursorLocked(false);
				Input::SetCursorVisible(true);
				SetAcceptInputDuringRuntime(false);
				break;

			}
		}

		return true;

	}
	bool OnWindowFocused(const Event& e) {

		WindowFocusedEvent* event = (WindowFocusedEvent*) & e;

		if (!event->focused || !data.scriptChanges) return true;
		data.project.BuildSolution();

		data.scriptChanges = false;

		return true;

	}
	bool OnWindowClose(const Event& e) {

		if (!data.changes) return true;

		Input::PopupResult test = Input::WarningPopup("Unsaved Changes", "There are Unsaved Changes in the project, do you wish to save the Project before exiting ?");

		switch (test) {

			case Input::PopupResult::Yes: {
				
				SaveScene();
				SaveEditorData();
				
				return true;
				break;
			
			}
			case Input::PopupResult::No: {
				
				return true;
				break;
			
			}
			case Input::PopupResult::Cancel: {

				return false;
				break;

			}
			default: {

				return false;
				break;

			}

		}

	}

	bool OnEntityCreated(const Event& e) {

		EntityEvent* event = (EntityEvent*) &e;

		return true;

	}
	bool OnEntityRemoved(const Event& e) {

		EntityEvent* event = (EntityEvent*) &e;

		return true;

	}
	
	const Project& GetProject() { return data.project; }
	SceneCamera& GetSceneCam() { return data.sceneCam; }

	MetaFile::SceneMeta* GetSceneMeta() { return &data.sceneMeta; }

	UVector2I GetViewportSize() { return data.viewportSize; }

	void SetChanges(bool value) {

		if (data.state == Play) return;

		data.changes = value;
		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene->name;
		data.title += '*';
		Input::SetWindowTitle(data.title);
		
	}

#ifdef CU_LINUX
	void RunPremake() {

		const std::string path = data.project.path.string();

		// It hurts my eyes, but there is no other solution that I know of
		//system(("cd \"" + data.project.path.String() + "\" ; ./premake/premake5 gmake2").c_str());

		// Turns out there is :)
		system(("./util/premake/premake5 --file=\"" + path + "premake5.lua\" gmake2").c_str());

	}
#endif

}

#pragma region EntryPoint
#include <Engine/Core/Entry.h>

void AppEntryPoint() {

	CUP_FUNCTION();

	// In the editor case, we have our own window that is bigger then the engine region
	// so we have to create and store it ourselves
	Editor::data.window = Window("Copper Editor", 1280, 720);

	AddPostInitEventFunc(Editor::Initialize);
	AddUIUpdateEventFunc(Editor::UIUpdate);
	AddPreShutdownEventFunc(Editor::OnWindowClose);
	AddPostShutdownEventFunc(Editor::Shutdown);

}
#pragma endregion 

Window* GetEditorWindow() { return &Editor::data.window; }
UVector2I GetViewportCentre() { return Editor::data.viewportCentre; }