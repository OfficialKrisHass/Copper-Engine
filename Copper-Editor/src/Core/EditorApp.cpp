#include "EditorApp.h"

#include "Engine/Utilities/Math.h"

#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"

#include "Viewport/SceneCamera.h"

#include "Core/Utils/ModelLoader.h"

#include <GLM/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#include <yaml-cpp/yaml.h>

#pragma region EntryPoint & Shutdown
#include <Engine/Core/Entry.h>

void AppEntryPoint() {

	Editor::Initialize();

	Copper::SetEditorRunFunc(Editor::Run);
	Copper::SetEditorUIFunc(Editor::UI);

	Copper::SetEditorOnKeyPressedFunc(Editor::OnKeyPressed);
	Copper::SetEditorOnWindowCloseFunc(Editor::OnWindowClose);

}
void AppShutdown() {

	Editor::Shutdown();

}
#pragma endregion 

namespace Editor {

	using namespace Copper;

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
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		//Back
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		//Right
		Vector3(-1.0f,  0.0f,  0.0f),
		Vector3(-1.0f,  0.0f,  0.0f),
		Vector3(-1.0f,  0.0f,  0.0f),
		Vector3(-1.0f,  0.0f,  0.0f),
		//Left
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
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
		int gizmoType = -1;

		//Project
		std::string projectName;

		std::filesystem::path projectPath;
		std::filesystem::path assetsPath;

		std::filesystem::path lastOpenedScene;

		//Scene
		Scene scene;
		bool changes;
		
		//Viewport
		UVector2I viewportSize;
		bool canLookViewport;

		//
		Texture playIcon;
		Texture stopIcon;
		
		//Panels
		SceneHierarchy sceneHierarchy;
		Properties properties;
		FileBrowser fileBrowser;
		
		//Objects
		SceneCamera sceneCam;

	};

	EditorData data;

	void OnEditorRuntimeStart();
	void OnEditorRuntimeUpdate();

	void Initialize() {

		LoadEditorData();
		LoadProjectData();

		data.state = Edit;
		data.viewportSize = UVector2I(1280, 720);
		//data.gizmoType = ImGuizmo::TRANSLATE;

		data.playIcon = Texture("assets/Icons/PlayButton.png");
		data.stopIcon = Texture("assets/Icons/StopButton.png");

		data.sceneHierarchy = SceneHierarchy();
		data.properties = Properties();
		data.fileBrowser = FileBrowser(data.assetsPath);
		
		data.sceneCam = SceneCamera(data.viewportSize);
		data.sceneCam.transform = new Transform(Vector3(0.0f, 0.0f, 1.0f), Vector3::zero, Vector3::one);
		data.sceneCam.transform->position.z = 1.0f;
		data.sceneCam.transform->parent = nullptr;
		
		//ManualScene();
		OpenScene(data.lastOpenedScene);

	}

	void Run() {

		if (data.state == Play) OnEditorRuntimeUpdate();

		if (Input::IsKey(KeyCode::Q) && !Input::IsButton(Input::Button2)) data.gizmoType = ImGuizmo::TRANSLATE;
		else if (Input::IsKey(KeyCode::W) && !Input::IsButton(Input::Button2)) data.gizmoType = ImGuizmo::ROTATE;
		else if (Input::IsKey(KeyCode::E) && !Input::IsButton(Input::Button2)) data.gizmoType = ImGuizmo::SCALE;

	}

	void UI() {

		RenderDockspace();
		RenderViewport();
		RenderToolbar();
		RenderMenu();
		
		data.fileBrowser.UIRender();
		data.sceneHierarchy.UIRender();
		data.properties.SetSelectedObject(data.sceneHierarchy.GetSelectedObject());
		data.properties.UIRender();

		ImGui::End(); //Dockspace

	}

	void Shutdown() {

		SaveProjectData();

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
	void RenderViewport() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		data.viewportSize = UVector2I((uint32_t) windowSize.x, (uint32_t) windowSize.y);
		SetWindowSize(data.viewportSize);
		
		ImGui::Image(reinterpret_cast<void*>((uint64_t) GetFBOTexture()), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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

			const glm::mat4& cameraProjection = data.sceneCam.CreateProjectionMatrix();
			glm::mat4 cameraView = data.sceneCam.CreateViewMatrix();
			glm::mat4 transform = selectedObj.transform->CreateMatrix();

			// Snapping
			bool snap = Input::IsKey(KeyCode::LeftControl);
			float snapValue = 0.5f;
			if (data.gizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;

			float snapValues[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
								 (ImGuizmo::OPERATION) data.gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
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
		data.sceneCam.SetCanLook(data.canLookViewport);
		
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

			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t) data.playIcon.GetID()), buttonSize, {0, 1}, {1, 0})) OnEditorRuntimeStart();

		} else if (data.state == Play) {

			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t) data.stopIcon.GetID()), buttonSize, {0, 1}, {1, 0})) data.state = Edit;

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
				if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S")) { SaveProjectData(); SaveScene(); }

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

				if (ImGui::DragFloat("Speed", &data.sceneCam.speed, 0.01f, 0.001f, 50.0f, "%.4f")) SetChanges(true);
				if (ImGui::DragFloat("Sensitivity", &data.sceneCam.sensitivity, 0.1f, 1.0f, 1000.0f)) SetChanges(true);

				ImGui::EndMenu();
				
			}

			ImGui::EndMenuBar();
			
		}
		
	}

	void OnEditorRuntimeStart() {

		data.state = Play;
		data.scene.OnRuntimeStart();

	}
	void OnEditorRuntimeUpdate() {

		data.scene.OnRuntimeUpdate();

	}

	void SaveEditorData() {

		YAML::Emitter out;

		out << YAML::BeginMap; //Start

		out << YAML::Key << "Last Project" << YAML::Value << data.projectPath.string();

		out << YAML::EndMap; //End

		std::ofstream file("assets/EditorData.cu");
		file << out.c_str();

	}
	void LoadEditorData() {

		YAML::Node main;
		try { main = YAML::LoadFile("assets/EditorData.cu"); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}

		data.projectPath = main["Last Project"].as<std::string>();

	}
	void SaveProjectData() {

		SaveEditorData();

		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "Name" << YAML::Value << data.projectName;
		out << YAML::Key << "Last Scene" << YAML::Value << data.lastOpenedScene.string();
		out << YAML::Key << "Gizmo" << YAML::Value << data.gizmoType;

		out << YAML::EndMap;

		std::ofstream file(data.projectPath.string() + "/Project.cu");
		file << out.c_str();

	}
	void LoadProjectData() {

		YAML::Node main;
		try { main = YAML::LoadFile(data.projectPath.string() + "/Project.cu"); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}

		data.assetsPath = data.projectPath.string() + "/Assets";
		data.projectName = main["Name"].as<std::string>();
		data.lastOpenedScene = data.assetsPath.string() + '/' + main["Last Scene"].as<std::string>();

		int gizmoType = main["Gizmo"].as<int>();
		data.gizmoType = gizmoType;

		data.title = "Copper Editor - " + data.projectName + ": ";
		Input::SetWindowTitle(data.title);

	}

	void NewProject() {

		std::filesystem::path path = Utilities::FolderOpenDialog();
		if (path.empty()) { LogWarn("Path is Invalid or empty"); return; }

		//Set the Project stuff
		data.projectPath = path;
		data.projectName = path.string().substr(path.string().find_last_of('\\') + 1);

		//Setup the Assets path and folder
		data.assetsPath = path.string() + "/Assets";
		std::filesystem::create_directories(data.assetsPath.string() + "/Scenes");
		data.fileBrowser.SetCurrentDir(data.assetsPath);

		//Create the Empty Template Scene
		data.scene = Scene();
		data.scene.name = "EmptyTemplate";

		LoadScene(&data.scene);

		Object light = data.scene.CreateObject("Light");
		light.transform->position.y = 1.0f;
		light.AddComponent<Light>();

		data.scene.cam = &data.sceneCam;

		//Finalize the Template Scene by Serializing it
		data.scene.Serialize(data.assetsPath.string() + "/Scenes/EmptyTemplate.copper");

		//Set the Last opened Scene path
		//TODO : There isn't really a reason why we need the LastOpenedScene path since it's in the
		//Project.cu file, but It's 11PM and I'm too lazy to change it now :)
		data.lastOpenedScene = std::filesystem::relative(data.scene.path, data.assetsPath);

		//Change the Title
		//TODO : Why is this here ? Am I always writing code at 11PM completely dead and insane ?
		data.title = "Copper Editor - " + data.projectName + ": ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);

		//Create the Project.cu file
		SaveProjectData();

	}
	void OpenProject() {

		std::filesystem::path path = Utilities::FolderOpenDialog();
		if (path.empty()) { LogWarn("Path is Invalid or empty"); return; }

		data.projectPath = path;
		LoadProjectData();
		data.fileBrowser.SetCurrentDir(data.assetsPath);

		OpenScene(data.lastOpenedScene);

	}

	void NewScene() {

		data.scene = Scene();
		data.scene.cam = &data.sceneCam;

		LoadScene(&data.scene);
		
	}
	void OpenScene() {

		std::filesystem::path path = Utilities::OpenDialog("Copper Scene (*.copper)\0*.copper\0", data.assetsPath);

		if(path.empty()) { LogWarn("The Path Specified is empty or is not a Copper Scene File"); return; }

		std::filesystem::path relativeToProjectAssets = std::filesystem::relative(path, data.assetsPath);
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

		data.lastOpenedScene = std::filesystem::relative(path, data.assetsPath);

		data.scene = Scene();
		LoadScene(&data.scene);

		data.scene.Deserialize(path);
		data.scene.cam = &data.sceneCam;

		data.changes = false;
		data.title = "Copper Editor - " + data.projectName + ": ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);
		
	}
	void SaveScene() {
		
		if(!data.scene.path.empty()) {

			data.scene.Serialize(data.scene.path);

			data.changes = false;
			data.title = "Copper Editor - " + data.projectName + ": ";
			data.title += data.scene.name;
			Input::SetWindowTitle(data.title);
			
			return;
			
		}

		SaveSceneAs();
		
	}
	void SaveSceneAs() {

		std::string path = Utilities::SaveDialog("Copper Scene (*.copper)\0*.copper\0", data.assetsPath);

		if(!path.empty()) {

			std::filesystem::path relativeToProjectAssets = std::filesystem::relative(path, data.assetsPath);
			if (relativeToProjectAssets.string()[0] == '.' && relativeToProjectAssets.string()[1] == '.') {

				switch (Input::Error::ErrorPopup("Invalid Scene", "The Place you want to save this scene is outside of this Project or starts with '..'")) {

					case IDOK: return;

				}

				return;

			}

			data.scene.Serialize(path);

			data.changes = false;
			data.title = "Copper Editor - " + data.projectName + ": ";
			data.title += data.scene.name;
			Input::SetWindowTitle(data.title);
			
		}
		
	}

	bool OnKeyPressed(Event& e) {

		bool control = Input::IsKey(KeyCode::LeftControl) || Input::IsKey(KeyCode::RightControl);
		bool shift = Input::IsKey(KeyCode::LeftShift) || Input::IsKey(KeyCode::RightShift);
		bool alt = Input::IsKey(KeyCode::LeftAlt) || Input::IsKey(KeyCode::RightAlt);

		KeyPresedEvent event = *(KeyPresedEvent*) &e;

		switch (event.key) {

			case KeyCode::S: {

				if (control && shift) { SaveProjectData(); break; }
				if (control && alt) { SaveSceneAs(); break; }
				if (control) { SaveScene(); break; }

			}

		}

		return true;

	}
	bool OnWindowClose(Event& e) {

		if (data.changes) {

			switch (Input::Error::WarningPopup("Unsaved Changes", "There are Unsaved Changes, if you close the Editor your changes will be lost. Are you Sure you want to Close the Editor ?")) {

				case IDOK: break;
				case IDCANCEL: return false;

			}

		}

		return true;

	}

	std::filesystem::path GetProjectPath() { return data.projectPath; }

	void SetChanges(bool value) {

		data.changes = value;
		data.title = "Copper Editor - " + data.projectName + ": ";
		data.title += data.scene.name;
		data.title += '*';
		Input::SetWindowTitle(data.title);
		
	}
	
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

		data.scene.cam = &data.sceneCam;

		data.changes = false;
		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);


	}

}