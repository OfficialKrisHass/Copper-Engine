#include "EditorApp.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Utilities/Math.h"

#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"

#include "Viewport/SceneCamera.h"

#include "Core/Utils/ModelLoader.h"

#include <GLM/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#pragma region EntryPoint
#include <Engine/Core/Entry.h>

void AppEntryPoint() {

	Editor::Initialize();

	Copper::SetEditorRunFunc(Editor::Run);
	Copper::SetEditorUIFunc(Editor::UI);

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

		data.state = Edit;
		data.viewportSize = UVector2I(1280, 720);
		data.gizmoType = ImGuizmo::TRANSLATE;

		data.playIcon = Texture("assets/Icons/PlayButton.png");
		data.stopIcon = Texture("assets/Icons/StopButton.png");

		data.sceneHierarchy = SceneHierarchy();
		data.properties = Properties();
		data.fileBrowser = FileBrowser(1);
		
		data.sceneCam = SceneCamera(data.viewportSize);
		data.sceneCam.transform = new Transform(Vector3(0.0f, 0.0f, 1.0f), Vector3::zero, Vector3::one);
		data.sceneCam.transform->position.z = 1.0f;
		data.sceneCam.transform->parent = nullptr;
		
		//ManualScene();
		OpenScene("assets/TestProject/Scenes/Default.copper");

		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);

	}

	void Run() {

		if (data.state == Play) OnEditorRuntimeUpdate();

		if (Input::IsKey(Input::Q)) data.gizmoType = ImGuizmo::TRANSLATE;
		else if (Input::IsKey(Input::W)) data.gizmoType = ImGuizmo::ROTATE;
		else if (Input::IsKey(Input::E)) data.gizmoType = ImGuizmo::SCALE; 

	}

	void UI() {

		RenderDockspace();
		RenderViewport();
		RenderMenu();
		
		data.fileBrowser.UIRender();
		data.sceneHierarchy.UIRender();
		data.properties.SetSelectedObject(data.sceneHierarchy.GetSelectedObject());
		data.properties.UIRender();

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
			bool snap = Input::IsKey(Input::LeftControl);
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
	void RenderMenu() {

		if(ImGui::BeginMenuBar()) {

			if(ImGui::BeginMenu("File")) {

				if(ImGui::MenuItem("New Scene", "Ctrl+N"))				NewScene();
				if(ImGui::MenuItem("Open Scene", "Ctrl+O"))				OpenScene();
				if(ImGui::MenuItem("Save Scene", "Ctr+S"))				SaveScene();
				if(ImGui::MenuItem("Save Ass", "Ctrl+Shift+S"))			SaveSceneAs();

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

	void NewScene() {

		data.scene = Scene();
		data.scene.cam = &data.sceneCam;

		LoadScene(&data.scene);
		
	}
	void OpenScene() {

		std::string path = Utilities::OpenDialog("Copper Scene (*.copper)\0*.copper\0");

		if(path.empty()) { LogWarn("The Path Specified is empty or is not a Copper Scene File"); return; }

		OpenScene(path);
		
	}
	void OpenScene(std::filesystem::path path) {

		if(data.changes) {

			switch(Input::Error::WarningPopup("Unsaved Changes", "There are Unsaved Changes, if you open another scene you will lose these Changes.")) {

			case IDOK: break;
			case IDCANCEL: return;
				
			}
			
		}

		data.scene = Scene();
		LoadScene(&data.scene);

		data.scene.Deserialize(path);
		data.scene.cam = &data.sceneCam;

		data.changes = false;
		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);
		
	}
	void SaveScene() {
		
		if(!data.scene.path.empty()) {

			data.scene.Serialize(data.scene.path);

			data.changes = false;
			data.title = "Copper Editor - TestProject: ";
			data.title += data.scene.name;
			Input::SetWindowTitle(data.title);
			
			return;
			
		}

		SaveSceneAs();
		
	}
	void SaveSceneAs() {

		std::string path = Utilities::SaveDialog("Copper Scene (*.copper)\0*.copper\0");

		if(!path.empty()) {

			data.scene.Serialize(path);

			data.changes = false;
			data.title = "Copper Editor - TestProject: ";
			data.title += data.scene.name;
			Input::SetWindowTitle(data.title);
			
		}
		
	}

	void SetChanges(bool value) {

		data.changes = value;
		data.title = "Copper Editor - TestProject: ";
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