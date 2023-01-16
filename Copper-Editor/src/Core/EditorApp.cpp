#include "EditorApp.h"

#include "Engine/Utilities/Math.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Renderer.h"

#include "Core/Project.h"
#include "Core/MetaFileSerialization.h"

#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"
#include "Panels/Console.h"

#include "Viewport/SceneCamera.h"

#include "Core/Utils/ModelLoader.h"

#include <GLM/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#include <yaml-cpp/yaml.h>

using namespace Copper;

#pragma region EntryPoint & Shutdown
#include <Engine/Core/Entry.h>

void AppEntryPoint() {

	Editor::Initialize();

	SetEditorRunFunc(Editor::Run);
	SetEditorUIFunc(Editor::UI);

	SetEditorOnKeyPressedFunc(Editor::OnKeyPressed);
	SetEditorOnWindowCloseFunc(Editor::OnWindowClose);

}
void AppShutdown() {

	Editor::Shutdown();

}
#pragma endregion 

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
		std::string title;

		//Project
		Project project;

		//Scene
		Scene scene;
		bool changes;
		
		//Viewport
		UVector2I viewportSize;
		FrameBuffer viewportFBO;
		bool canLookViewport;

		//Game Panel
		UVector2I gamePanelSize;

		//Icons
		Texture playIcon;
		Texture stopIcon;
		
		//Panels
		SceneHierarchy sceneHierarchy;
		Properties properties;
		FileBrowser fileBrowser;
		Console console;

	};

	EditorData data;

	void SaveEditorData();
	void LoadEditorData();

	void NewProject();
	void OpenProject(std::filesystem::path path);
	void OpenProject();

	void CopyScriptingAPI();

	void StartEditorRuntime();
	void StopEditorRuntime();

	void Initialize() {

		data.state = Edit;
		data.viewportSize = UVector2I(1280, 720);
		data.gamePanelSize = UVector2I(1280, 720);
		data.viewportFBO = FrameBuffer(data.viewportSize);
		
		data.playIcon = Texture("assets/Icons/PlayButton.png");
		data.stopIcon = Texture("assets/Icons/StopButton.png");
		
		data.sceneHierarchy = SceneHierarchy();
		data.properties = Properties();
		data.fileBrowser = FileBrowser(data.project.assetsPath);
		data.console = Console();
		
		LoadEditorData();

	}
	void Run() {



	}
	void Shutdown() {

		SaveEditorData();

	}

	void SaveEditorData() {

		YAML::Emitter out;

		out << YAML::BeginMap; //Start

		out << YAML::Key << "Last Project" << YAML::Value << data.project.path.string();

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

	void UI() {

		RenderDockspace();
		RenderToolbar();
		RenderMenu();
		
		data.console.UIRender();
		data.fileBrowser.UIRender();
		data.sceneHierarchy.UIRender();
		data.properties.SetSelectedObject(data.sceneHierarchy.GetSelectedObject());
		data.properties.UIRender();
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

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruDockspace) window_flags |= ImGuiWindowFlags_NoBackground;

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
		if (!data.scene.cam) {

			ImGui::Text("No Camera Available!");

			ImGui::End();
			ImGui::PopStyleVar();

			return;

		}

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		data.gamePanelSize = UVector2I((uint32_t) windowSize.x, (uint32_t) windowSize.y);

		SetWindowSize(data.gamePanelSize);

		ImGui::Image(reinterpret_cast<void*>((uint64_t) GetFBOTexture()), windowSize, ImVec2 {0, 1}, ImVec2 {1, 0});

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

		//TODO: Either Change ImGui To use UVector2I or edit Copper Code to use ImVec2
		//      so that we don't have to allocate memory for the UVector2I
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		data.viewportSize = UVector2I((uint32_t) windowSize.x, (uint32_t) windowSize.y);

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
		data.scene.Render(&data.project.sceneCam);

		//After we are done rendering we are safe to unbind the FBO unless we want to modify it any way
		data.viewportFBO.Unbind();

		ImGui::Image(reinterpret_cast<void*>((uint64_t) data.viewportFBO.GetColorAttachment()), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		//Gizmos that I stol... I mean, taken inspiration from The Chernos Game Engine series
		//Yeah, I definitely didn't copy this entire chunk of code that I don't understand but
		//magically works, naaah.
		Object selectedObj = data.sceneHierarchy.GetSelectedObject();
		if (selectedObj) {

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float wWidth = (float) ImGui::GetWindowWidth();
			float wHeight = (float) ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, wWidth, wHeight);

			const glm::mat4& cameraProjection = data.project.sceneCam.CreateProjectionMatrix();
			glm::mat4 cameraView = data.project.sceneCam.CreateViewMatrix();
			glm::mat4 transform = selectedObj.transform->CreateMatrix();

			// Snapping
			bool snap = Input::IsKey(KeyCode::LeftControl);
			float snapValue = 0.5f;
			if (data.project.gizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;

			float snapValues[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
								 (ImGuizmo::OPERATION) data.project.gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
								 nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {

				glm::vec3 position, rotation, scale;

				Math::DecomposeTransform(transform, position, rotation, scale);

				glm::vec3 deltaRotation = rotation - (glm::vec3) selectedObj.transform->rotation;
				selectedObj.transform->position = position;
				selectedObj.transform->rotation += deltaRotation;
				selectedObj.transform->scale = scale;

				//The rotation doesn't work for some reason, it keeps wiggling around
				//Unfortunately I'm dum dum so this is what you get :) uwu

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
				if(ImGui::MenuItem("Save Project", "Ctrl+Shift+S")) { data.project.Save(); SaveEditorData(); SaveScene(); }

				ImGui::Separator();

				if (ImGui::MenuItem("Build Solution", "Ctrl+B")) data.project.BuildSolution();
				
				ImGui::Separator();

				if (ImGui::MenuItem("Create Solution Files")) data.project.CreateSolution();

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
		data.scene.StartRuntime();

	}
	void StopEditorRuntime() {

		data.state = Edit;

		std::filesystem::path savedPath = data.scene.path;

		data.scene = Scene();
		data.scene.Deserialize(savedPath);
		data.scene.cam = &data.project.sceneCam;

	}

	void NewProject() {

		//Open the Folder Dialog
		//TODO : Either make our own Folder Open Dialog or Start using the Windows new System
		std::filesystem::path path = Utilities::FolderOpenDialog();
		if (path.empty()) { LogWarn("Path is Invalid or Empty"); return; }

		//Create the Project
		data.project = Project(path.string().substr(path.string().find_last_of('\\') + 1), path);

		//Create the Directioes
		std::filesystem::create_directories(path.string() + "\\Assets\\Scenes");
		std::filesystem::create_directories(path.string() + "\\Binaries");
		std::filesystem::create_directories(path.string() + "\\Objs");

		//Setup the FileBrowser
		data.fileBrowser.SetCurrentDir(data.project.assetsPath);

		//Copy the Template scene
		std::ifstream templateScene("assets/Projects/EmptyTemplate/Assets/Scenes/EmptyTemplate.copper");
		std::fstream projectScene;

		projectScene.open(data.project.assetsPath.string() + "\\Scenes\\EmptyTemplate.copper", std::ios::out);
		projectScene << templateScene.rdbuf();
		projectScene.close();
		data.project.lastOpenedScene = "Scenes\\EmptyTemplate.copper";

		//Copy the ScriptingAPI dll
		CopyScriptingAPI();

		//Create, Build and Load the Solution and Assembly
		data.project.CreateSolution();
		data.project.BuildSolution(false);

		Scripting::LoadProjectAssembly(data.project.path.string() + "\\Binaries\\" + data.project.name + ".dll");
		Scripting::Reload();

		//Lastly Save the Project into the Project.cu file
		data.project.Save();

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": ";
		Input::SetWindowTitle(data.title);

	}
	void OpenProject(std::filesystem::path path) {

		data.project = Project("", path);
		data.project.Load();

		data.fileBrowser.SetCurrentDir(data.project.assetsPath);

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": ";
		Input::SetWindowTitle(data.title);

		Scripting::LoadProjectAssembly(data.project.path.string() + "\\Binaries\\" + data.project.name + ".dll");
		Scripting::Reload();

		OpenScene(data.project.lastOpenedScene);

	}
	void OpenProject() {

		std::filesystem::path path = Utilities::FolderOpenDialog();
		if (path.empty()) { LogWarn("path is Invalid or empty"); return; }

		OpenProject(path);

	}

	void CopyScriptingAPI() {

		std::ifstream dllSrc("assets/ScriptAPI/Copper-ScriptingAPI.dll", std::ios::binary);
		std::fstream dllDst;

		dllDst.open(data.project.path.string() + "/Binaries/Copper-ScriptingAPI.dll", std::ios::out | std::ios::binary);
		dllDst << dllSrc.rdbuf();
		dllDst.close();

	}

	void NewScene() {

		data.scene = Scene();

		LoadScene(&data.scene);
		data.sceneHierarchy.SetScene(&data.scene);
		
	}
	void OpenScene() {

		std::filesystem::path path = Utilities::OpenDialog("Copper Scene (*.copper)\0*.copper\0", "assets/Projects/DevProject/Assets");

		if(path.empty()) { LogWarn("The Path Specified is empty or is not a Copper Scene File"); return; }

		std::filesystem::path relativeToProjectAssets = std::filesystem::relative(path, "assets/Projects/DevProject/Assets");
		if (relativeToProjectAssets.string()[0] == '.' && relativeToProjectAssets.string()[1] == '.') {

			switch (Input::Error::ErrorPopup("Invalid Scene", "The Scene you have selected is outside your project, or in a folder starting with '..'")) {

				case IDOK: return;

			}

			return;

		}

		OpenScene(path);
		
	}
	void OpenScene(std::filesystem::path path) {

		if(data.changes) {

			switch(Input::Error::WarningPopup("Unsaved Changes", "There are Unsaved Changes, if you open another scene you will lose these Changes.")) {

			case IDOK: break;
			case IDCANCEL: return;
				
			}
			
		}

		data.project.lastOpenedScene = std::filesystem::relative(path, data.project.assetsPath);

		data.scene = Scene();
		LoadScene(&data.scene);

		data.scene.Deserialize(path);
		data.sceneHierarchy.SetScene(&data.scene);
		data.sceneHierarchy.LoadSceneMeta();

		data.changes = false;
		data.title = "Copper Editor - " + data.project.name + ": ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);
		
	}
	void SaveScene() {
		
		if(!data.scene.path.empty()) {

			data.scene.Serialize(data.scene.path);
			data.sceneHierarchy.SaveSceneMeta();

			data.changes = false;
			data.title = "Copper Editor - TestProject: ";
			data.title += data.scene.name;
			Input::SetWindowTitle(data.title);
			
			return;
			
		}

		SaveSceneAs();
		
	}
	void SaveSceneAs() {

		std::string path = Utilities::SaveDialog("Copper Scene (*.copper)\0*.copper\0", "assets/Projects/DevProject/Assets");

		if(!path.empty()) {

			std::filesystem::path relativeToProjectAssets = std::filesystem::relative(path, data.project.assetsPath);
			if (relativeToProjectAssets.string()[0] == '.' && relativeToProjectAssets.string()[1] == '.') {

				switch (Input::Error::ErrorPopup("Invalid Scene", "The Place you want to save this scene is outside of this Project or starts with '..'")) {

					case IDOK: return;

				}

				return;

			}

			data.scene.Serialize(path);
			data.sceneHierarchy.SaveSceneMeta();

			data.changes = false;
			data.title = "Copper Editor - TestProject: ";
			data.title += data.scene.name;
			Input::SetWindowTitle(data.title);
			
		}
		
	}

	bool OnKeyPressed(const Event& e) {

		if (data.state == Play) return true;

		bool control = Input::IsKey(KeyCode::LeftControl) || Input::IsKey(KeyCode::RightControl);
		bool shift = Input::IsKey(KeyCode::LeftShift) || Input::IsKey(KeyCode::RightShift);
		bool alt = Input::IsKey(KeyCode::LeftAlt) || Input::IsKey(KeyCode::RightAlt);

		bool rightClick = Input::IsButton(Input::Button2);

		KeyPresedEvent event = *(KeyPresedEvent*) &e;

		switch (event.key) {

			case KeyCode::S: {

				if (control && shift) { data.project.Save(); SaveEditorData(); SaveScene(); }
				if (control && alt) SaveSceneAs();
				if (control) SaveScene();

				break;

			}
			case KeyCode::B: {

				if (control) data.project.BuildSolution();

				break;

			}
			case KeyCode::Q: {

				if (!rightClick) data.project.gizmoType = ImGuizmo::TRANSLATE;

				break;

			}
			case KeyCode::W: {

				if (!rightClick) data.project.gizmoType = ImGuizmo::ROTATE;

				break;

			}
			case KeyCode::E: {

				if (!rightClick) data.project.gizmoType = ImGuizmo::SCALE;

				break;

			}
		}

		return true;

	}
	bool OnWindowClose(const Event& e) {

		if (data.changes) {

			switch (Input::Error::WarningPopup("Unsaved Changes", "There are Unsaved Changes, if you close the Editor your changes will be lost. Are you Sure you want to Close the Editor ?")) {

				case IDOK: break;
				case IDCANCEL: return false;

			}

		}

		return true;

	}

	void SetChanges(bool value) {

		data.changes = value;
		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene.name;
		data.title += '*';
		Input::SetWindowTitle(data.title);
		
	}
	
	Project GetProject() { return data.project; }

	UVector2I GetViewportSize() { return data.viewportSize; }

	void ManualScene() {

		data.scene = Scene();
		data.scene.name = "Manual";
		data.scene.path = "assets/TestProject/Test Scenes/Manual.copper";

		LoadScene(&data.scene);

		Object square = data.scene.CreateObject(Vector3::zero, Vector3(90.0f, 0.0f, 0.0f), Vector3::one, "Square");
		Object cube = data.scene.CreateObject(Vector3(1.5f, 0.0f, 0.0f), Vector3::zero, Vector3::one, "Cube");
		Object light = data.scene.CreateObject(Vector3(0.0f, 0.0f, 1.0f), Vector3::zero, Vector3::one, "Light");

		cube.transform->parent = square.transform;
		square.transform->AddChild(cube.transform);

		MeshRenderer* renderer = square.AddComponent<MeshRenderer>();
		Mesh mesh;

		mesh.vertices = planeVertices;
		mesh.normals  = planeNormals;
		mesh.colors   = planeColors;
		mesh.indices  = planeIndices;

		renderer->meshes.push_back(mesh);

		renderer = cube.AddComponent<MeshRenderer>();
		
		mesh.vertices = cubeVertices;
		mesh.normals  = cubeNormals;
		mesh.colors   = cubeColors;
		mesh.indices  = cubeIndices;

		renderer->meshes.push_back(mesh);

		light.AddComponent<Light>();

		data.changes = false;
		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);


	}

}