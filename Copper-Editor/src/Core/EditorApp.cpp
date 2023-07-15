#include "EditorApp.h"

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

#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"
#include "Panels/Console.h"

#include "Viewport/SceneCamera.h"

#include <GLM/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#include <yaml-cpp/yaml.h>
#include "Engine/YAMLOverloads/YAMLOverloads.h"

#include <CMath/CMath.h>

using namespace Copper;

namespace Editor {

#pragma region Plane
	std::vector<Vector3> planeVertices {

		Vector3(-0.5f, 0.0f,  0.5f),
		Vector3(0.5f, 0.0f,  0.5f),
		Vector3(0.5f, 0.0f, -0.5f),
		Vector3(-0.5f, 0.0f, -0.5f),

	};
	std::vector<Vector3> planeNormals {

		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f),

	};
	std::vector<Color> planeColors {

		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),

	};
	std::vector<uint32_t> planeIndices {

		0, 1, 2,
		2, 3, 0

	};
#pragma endregion
#pragma region Cube
	std::vector<Vector3> cubeVertices {
		//Front
		Vector3(-0.5f, -0.5f,  0.5f),
		Vector3(0.5f, -0.5f,  0.5f),
		Vector3(0.5f,  0.5f,  0.5f),
		Vector3(-0.5f,  0.5f,  0.5f),
		//Back
		Vector3(0.5f, -0.5f, -0.5f),
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(-0.5f,  0.5f, -0.5f),
		Vector3(0.5f,  0.5f, -0.5f),
		//Right
		Vector3(0.5f, -0.5f,  0.5f),
		Vector3(0.5f, -0.5f, -0.5f),
		Vector3(0.5f,  0.5f, -0.5f),
		Vector3(0.5f,  0.5f,  0.5f),
		//Left
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(-0.5f, -0.5f,  0.5f),
		Vector3(-0.5f,  0.5f,  0.5f),
		Vector3(-0.5f,  0.5f, -0.5f),
		//Up
		Vector3(-0.5f,  0.5f,  0.5f),
		Vector3(0.5f,  0.5f,  0.5f),
		Vector3(0.5f,  0.5f, -0.5f),
		Vector3(-0.5f,  0.5f, -0.5f),
		//Down
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(0.5f, -0.5f, -0.5f),
		Vector3(0.5f, -0.5f,  0.5f),
		Vector3(-0.5f, -0.5f,  0.5f),

	};
	std::vector<Vector3> cubeNormals {
		//Front
		Vector3( 0.0f,  0.0f,  1.0f),
		Vector3( 0.0f,  0.0f,  1.0f),
		Vector3( 0.0f,  0.0f,  1.0f),
		Vector3( 0.0f,  0.0f,  1.0f),
		//Back
		Vector3( 0.0f,  0.0f, -1.0f),
		Vector3( 0.0f,  0.0f, -1.0f),
		Vector3( 0.0f,  0.0f, -1.0f),
		Vector3( 0.0f,  0.0f, -1.0f),
		//Right
		Vector3( 1.0f,  0.0f,  0.0f),
		Vector3( 1.0f,  0.0f,  0.0f),
		Vector3( 1.0f,  0.0f,  0.0f),
		Vector3( 1.0f,  0.0f,  0.0f),
		//Left
		Vector3(-1.0f,  0.0f,  0.0f),
		Vector3(-1.0f,  0.0f,  0.0f),
		Vector3(-1.0f,  0.0f,  0.0f),
		Vector3(-1.0f,  0.0f,  0.0f),
		//Up
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		//Down
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f),

	};
	std::vector<Color> cubeColors {
		//Front
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		//Back
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		//Right
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		//Left
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		//Up
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		//Down
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),

	};
	std::vector<uint32_t> cubeIndices {

		//Front
		0, 1, 2,
		2, 3, 0,

		//Back
		4, 5, 6,
		6, 7, 4,

		//Right
		8, 9, 10,
		10, 11, 8,

		//Left
		12, 13, 14,
		14, 15, 12,

		//Up
		16, 17, 18,
		18, 19, 16,

		//Down
		20, 21, 22,
		22, 23, 20

	};
#pragma endregion 

	struct EditorData {

		EditorState state;
		Window window;
		std::string title;

		//Project
		Project project;
		bool scriptChanges;

		//Scene
		Scene* scene;
		MetaFile::SceneMeta sceneMeta;
		bool changes;
		
		//Viewport
		UVector2I viewportSize;
		UVector2I viewportCentre;
		FrameBuffer viewportFBO;
		bool canLookViewport;

		//Game Panel
		UVector2I gamePanelSize;
		bool gamePanelFocused;

		//Icons
		Texture playIcon;
		Texture stopIcon;
		
		//Panels
		SceneHierarchy sceneHierarchy;
		Properties properties;
		FileBrowser fileBrowser;
		Console console;

		//Scripting
		bool wasCursorLocked = false;
		bool wasCursorVisible = true;

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
	void OpenProject(const Filesystem::Path& path);
	void OpenProject();

	void FileChangedCallback(const Filesystem::Path& path, const ProjectFileWatcher::FileChangeType& changeType);
	void CopyScriptingAPI();

	void StartEditorRuntime();
	void StopEditorRuntime();

	bool OnKeyPressed(const Event& e);
	bool OnWindowClose(const Event& e);
	bool OnWindowFocused(const Event& e);

	bool OnEntityCreated(const Event& e);
	bool OnEntityRemoved(const Event& e);

	void Initialize() {

		GetWindow().AddWindowFocusedEventFunc(Editor::OnWindowFocused);
		GetWindow().AddKeyPressedEventFunc(Editor::OnKeyPressed);

		AddEntityCreatedEventFunc(OnEntityCreated);
		AddEntityRemovedEventFunc(OnEntityRemoved);

		LoadUIFont("assets/Fonts/open-sans.regular.ttf");

		data.state = Edit;
		data.viewportSize = UVector2I(1280, 720);
		data.gamePanelSize = UVector2I(1280, 720);
		data.viewportFBO = FrameBuffer(data.viewportSize);
		
		data.playIcon = Texture("assets/Icons/PlayButton.png");
		data.stopIcon = Texture("assets/Icons/StopButton.png");
		
		data.sceneHierarchy = SceneHierarchy();
		data.properties = Properties();
		data.fileBrowser = FileBrowser("");
		data.console = Console();

		data.properties.SetSelectedObject(data.sceneHierarchy.GetSelectedEntity());

		ProjectFileWatcher::AddFilter("cs");
		ProjectFileWatcher::AddFileChangeCallback(FileChangedCallback);
		
		LoadEditorData();

	}
	void Shutdown() {

		SaveEditorData();

	}

	void SaveEditorData() {

		YAML::Emitter out;

		out << YAML::BeginMap; //Start

		out << YAML::Key << "Last Project" << YAML::Value << data.project.path;

		out << YAML::EndMap; //End

		std::ofstream file("assets/EditorData.cu");
		file << out.c_str();

		data.project.Save();

	}
	void LoadEditorData() {

		YAML::Node main;
		try { main = YAML::LoadFile("assets/EditorData.cu"); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}

		OpenProject(main["Last Project"].as<std::string>());

	}

	void UIUpdate() {
		
		RenderDockspace();
		RenderToolbar();
		RenderMenu();
		
		data.console.UIRender();
		data.fileBrowser.UIRender();
		data.properties.UIRender();
		data.sceneHierarchy.UIRender();
		RenderGamePanel();
		RenderViewport();

		ImGui::End(); //Dockspace

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

			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

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

		//Imgui::Begin returns a bool based on if the Window is visible/open
		//So, we store that and then Render the scene and window Only if it's visible
		//to save some performance
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
		bool open = ImGui::Begin("Game");
		SetRenderScene(open);

		if (!open) {

			ImGui::End();
			ImGui::PopStyleVar();

			return;

		}
		if (!data.scene->cam) {

			ImGui::Text("No Camera Available!");

			ImGui::End();
			ImGui::PopStyleVar();

			return;

		}

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		data.gamePanelSize = UVector2I((uint32_t) windowSize.x, (uint32_t) windowSize.y);

		SetWindowSize(data.gamePanelSize);

		ImGui::Image(reinterpret_cast<void*>((uint64_t) GetFBOTexture()), windowSize, ImVec2 {0, 1}, ImVec2 {1, 0});

		if (ImGui::IsItemClicked() && IsRuntimeRunning()) {
			
			Input::SetCursorLocked(data.wasCursorLocked);
			Input::SetCursorVisible(data.wasCursorVisible);
			SetAcceptInputDuringRuntime(true);
		
		}

		ImGui::End();
		ImGui::PopStyleVar();

	}
	void RenderViewport() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		bool open = ImGui::Begin("Viewport");
		if (!open) {

			ImGui::End();
			ImGui::PopStyleVar();

			return;

		}

		ImGui::GetWindowPos();

		//TODO: Either Change ImGui To use UVector2I or edit Copper Code to use ImVec2
		//      so that we don't have to allocate memory for the UVector2I
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImVec2 windowPos = ImGui::GetWindowPos();

		data.viewportSize = UVector2I((uint32_t) windowSize.x, (uint32_t) windowSize.y);

		data.viewportCentre = data.viewportSize / 2;
		data.viewportCentre.x += (uint32_t) windowPos.x;
		data.viewportCentre.y += (uint32_t) windowPos.y;

		if (data.viewportFBO.Width() != data.viewportSize.x || data.viewportFBO.Height() != data.viewportSize.y) {

			//We don't need to Call SetWindowSize because if the Viewport size is changed
			//it only affects the Viewport, not the Actualy Game Engine and the Main Game Panel
			data.viewportFBO.Resize(data.viewportSize);
			data.project.sceneCam.Resize(data.viewportSize);

		}

		//We need to Clear the Color because if we don't we just get a black image
		data.viewportFBO.Bind();
		Renderer::ClearColor(0.18f, 0.18f, 0.18f);

		data.project.sceneCam.Update();
		data.scene->Render(&data.project.sceneCam);

		//After we are done rendering we are safe to unbind the FBO unless we want to modify it any way
		data.viewportFBO.Unbind();

		ImGui::Image(reinterpret_cast<void*>((uint64_t) data.viewportFBO.GetColorAttachment()), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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

			Matrix4 camProjection = data.project.sceneCam.CreateProjectionMatrix();
			Matrix4 camView = data.project.sceneCam.CreateViewMatrix();
			glm::mat4 transform = selectedObj->GetTransform()->CreateMatrix();

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
				selectedObj->GetTransform()->position = position;
				//selectedObj->GetTransform()->rotation += deltaRotation;
				selectedObj->GetTransform()->scale = scale;

				//The rotation doesn't work for some reason, it keeps wiggling around
				//Unfortunately I'm dum dum so this is what you get :) uwu

				//TODO: Dont be dum dum like a friggin idiot and learn how to do it owo

			}

		}

		data.canLookViewport = ImGui::IsItemHovered();
		data.project.sceneCam.SetCanLook(data.canLookViewport);
		
		ImGui::End();
		ImGui::PopStyleVar();

	}
	void RenderToolbar() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

		auto& colors = ImGui::GetStyle().Colors;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(colors[ImGuiCol_ButtonHovered].x, colors[ImGuiCol_ButtonHovered].y, colors[ImGuiCol_ButtonHovered].z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(colors[ImGuiCol_ButtonActive].x, colors[ImGuiCol_ButtonActive].y, colors[ImGuiCol_ButtonActive].z, 0.5f));

		ImGui::Begin("##ToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImVec2 buttonSize = ImVec2(25, 25);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - buttonSize.x) * 0.5f);

		if (data.state == Edit) {

			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t) data.playIcon.GetID()), buttonSize, {0, 1}, {1, 0})) StartEditorRuntime();

		} else if (data.state == Play) {

			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t) data.stopIcon.GetID()), buttonSize, {0, 1}, {1, 0})) StopEditorRuntime();

		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();

	}
	void RenderMenu() {

		if(ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("Project")) {

				if (ImGui::MenuItem("New Project")) NewProject();
				if (ImGui::MenuItem("Open Project")) OpenProject();
				if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S")) { data.project.Save(); SaveEditorData(); SaveScene(); }

				ImGui::Separator();

				if (ImGui::MenuItem("Create Template")) CreateTemplateFromProject(data.project);

				ImGui::Separator();

				if (ImGui::MenuItem("Build Solution", "Ctrl+B")) data.project.BuildSolution();
				
				ImGui::Separator();

				if (ImGui::MenuItem("Copy Copper Scripting API")) CopyScriptingAPI();

				ImGui::EndMenu();

			}

			if(ImGui::BeginMenu("File")) {

				if(ImGui::MenuItem("New Scene"))				NewScene();
				if(ImGui::MenuItem("Open Scene"))				OpenScene();
				if(ImGui::MenuItem("Save Scene", "Ctr+S"))		SaveScene();
				if(ImGui::MenuItem("Save Ass", "Ctrl+Alt+S"))	SaveSceneAs();

				ImGui::EndMenu();
				
			}

			if(ImGui::BeginMenu("Camera")) {

				if (ImGui::DragFloat("Speed", &data.project.sceneCam.speed, 0.01f, 0.001f, 50.0f, "%.4f")) SetChanges(true);
				if (ImGui::DragFloat("Sensitivity", &data.project.sceneCam.sensitivity, 0.1f, 1.0f, 1000.0f)) SetChanges(true);

				ImGui::EndMenu();
				
			}

			ImGui::EndMenuBar();
			
		}
		
	}

	void StartEditorRuntime() {

		data.state = Play;

		SaveScene();
		data.scene->StartRuntime();

		Input::SetCursorLocked(true);
		Input::SetCursorVisible(false);
		SetAcceptInputDuringRuntime(true);

	}
	void StopEditorRuntime() {

		if (data.wasCursorLocked)
			Input::SetCursorLocked(false);

		data.state = Edit;
		Filesystem::Path savedPath = data.scene->path;
		Entity savedSelectedEntity = *data.sceneHierarchy.GetSelectedEntity();

		data.scene = GetScene();
		data.scene->Deserialize(savedPath);
		data.sceneMeta.Deserialize(data.scene);

		data.sceneHierarchy.SetSelectedEntity(savedSelectedEntity);

		Input::SetCursorLocked(false);
		Input::SetCursorVisible(true);
		SetAcceptInputDuringRuntime(false);

	}

	void FileChangedCallback(const Filesystem::Path& path, const ProjectFileWatcher::FileChangeType& changeType) {

		data.scriptChanges = true;

	}

	void NewProject() {

		//Open the Folder Dialog
		//TODO : Either make our own Folder Open Dialog or Start using the Windows new System
		//TODO : Fixed... well for linux
		Filesystem::Path path = Utilities::FolderOpenDialog((!data.project.path.Empty()) ? data.project.path.ParentPath() : ROOT_DIR);
		if (path.Empty()) { LogWarn("Path is Invalid or Empty"); return; }

		//Create the Project
		data.project = Project(path.File().String(), path);
		FileBrowser::SetRelativeDir("");

		CreateProjectFromTemplate("assets/Templates/DevProject", data.project);

		//Setup the FileBrowser
		data.project.BuildSolution(true);

		OpenScene(data.project.assetsPath / data.project.lastOpenedScene);

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": EmptyTemplate";
		Input::SetWindowTitle(data.title);

	}
	void OpenProject(const Filesystem::Path& path) {
		
		data.project.Load(path);
		data.scene = GetScene();
		FileBrowser::SetRelativeDir("");

		ProjectFileWatcher::Stop();
		ProjectFileWatcher::SetDirectory(data.project.assetsPath);
		ProjectFileWatcher::Start();

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": ";
		Input::SetWindowTitle(data.title);

		Scripting::Reload(data.project.path / "Binaries" / data.project.name + ".dll", false);

		OpenScene(data.project.lastOpenedScene);

	}
	void OpenProject() {

		Filesystem::Path path = Utilities::FolderOpenDialog((!data.project.path.Empty()) ? data.project.path.ParentPath() : ROOT_DIR);
		if (path.Empty()) { LogWarn("path is Invalid or empty"); return; }

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
		data.sceneMeta.objectIDs.clear();

		data.sceneHierarchy.SetScene(data.scene);
		
	}
	void OpenScene(const Filesystem::Path& path) {

		if(data.changes) {

			switch(Input::WarningPopup("Unsaved Changes", "There are Unsaved Changes, if you open another scene you will lose these Changes.")) {

			case Input::PopupResult::Ok: break;
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

		data.project.lastOpenedScene = path.RelativeTo(data.project.assetsPath);
		
	}
	void OpenScene() {

		Filesystem::Path path = Utilities::OpenDialog("Copper Scene (*.copper)\n*.copper", data.project.assetsPath);

		if(path.Empty()) { LogWarn("The Path Specified is empty or is not a Copper Scene File\n {}", path); return; }

		Filesystem::Path relativeToProjectAssets = path.RelativeTo(data.project.assetsPath);
		if (relativeToProjectAssets.Empty()) {

			Input::ErrorPopup("Invalid Scene Path", "The scene you have tried to Open is outside of the Assets folder of this Project.");
			return;

		}

		OpenScene(path);
		
	}
	void SaveScene() {
		
		if(!data.scene->path.Empty()) {

			data.scene->Serialize(data.scene->path);
			data.sceneMeta.Serialize(data.scene);

			data.changes = false;
			data.title = "Copper Editor - TestProject: ";
			data.title += data.scene->name;
			Input::SetWindowTitle(data.title);
			
			return;
			
		}

		SaveSceneAs();
		
	}
	void SaveSceneAs() {

		Filesystem::Path path = Utilities::SaveDialog("Copper Scene (*.copper)\n*.copper", data.project.assetsPath);

		if(!path.Empty()) {

			Filesystem::Path relativeToProjectAssets = path.RelativeTo(data.project.assetsPath);
			if (relativeToProjectAssets.Empty()) {

				Input::ErrorPopup("Invalid Scene", "The Place you want to save this scene is outside of this Project or starts with '..'");
				return;

			}

			data.scene->Serialize(path);
			data.sceneMeta.Serialize(data.scene);

			data.changes = false;
			data.title = "Copper Editor - TestProject: ";
			data.title += data.scene->name;
			Input::SetWindowTitle(data.title);
			
		}
		
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

		//Input::WarningPopup("Test Error", "With a random ass Description uwu");

		if (!data.changes) return true;

		switch (Input::WarningPopup("Unsaved Changes", "There are Unsaved Changes, if you close the Editor your changes will be lost. Are you Sure you want to Close the Editor ?")) {

			case Input::PopupResult::Ok: return true;
			case Input::PopupResult::Cancel: return false;

		}

		return true;

	}

	bool OnEntityCreated(const Event& e) {

		EntityEvent* event = (EntityEvent*) &e;

		data.sceneMeta.objectIDs.push_back(event->entity.ID());

		return true;

	}
	bool OnEntityRemoved(const Event& e) {

		EntityEvent* event = (EntityEvent*) &e;

		for (int i = 0; i < data.sceneMeta.objectIDs.size(); i++) {

			if (data.sceneMeta.objectIDs[i] != event->entity.ID()) continue;
			data.sceneMeta.objectIDs.erase(data.sceneMeta.objectIDs.begin() + i);
			break;

		}

		return true;

	}
	
	Project GetProject() { return data.project; }

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

}

#pragma region EntryPoint
#include <Engine/Core/Entry.h>

void AppEntryPoint() {

	Editor::data.window = Window("Copper Editor", 1280, 720);

	AddPostInitEventFunc(Editor::Initialize);
	AddUIUpdateEventFunc(Editor::UIUpdate);
	AddPreShutdownEventFunc(Editor::OnWindowClose);
	AddPostShutdownEventFunc(Editor::Shutdown);

}
#pragma endregion 

Window* GetEditorWindow() { return &Editor::data.window; }
UVector2I GetViewportCentre() { return Editor::data.viewportCentre; }