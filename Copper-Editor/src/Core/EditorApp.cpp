#include "EditorApp.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Object.h"

#include "Engine/Scene/Components/Mesh.h"

#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"
#include "Panels/FileBrowser.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>

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

	std::vector<float> vertices{
		//Position				//Color				//Normals
		-0.5f, -0.5f,  0.0f,    1.0f, 0.0f, 0.0f,	0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.0f,    0.0f, 0.0f, 1.0f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 1.0f,	0.0f,  0.0f, -1.0f,

	};

	std::vector<uint32_t> indices{

		0, 1, 2,
		2, 3, 0,

	};

	struct EditorData {

		std::shared_ptr<FrameBuffer> fbo;
		std::string title;

		//Scene
		Scene scene;
		bool changes;
		
		//Viewport
		UVector2I viewportSize;
		bool canLookViewport;
		
		//Panels
		SceneHierarchy sceneHierarchy;
		Properties properties;
		FileBrowser fileBrowser;
		
		//Objects
		SceneCamera sceneCam;

	};

	EditorData data;

	void Initialize() {

		Log("--------------------Editor Initialization");
		Log("Editor Initialization Started!");

		data.viewportSize = UVector2I(1280, 720);
		data.fbo = std::make_shared<FrameBuffer>(data.viewportSize);

		data.sceneHierarchy = SceneHierarchy();
		data.properties = Properties();
		data.fileBrowser = FileBrowser(1);
		
		data.sceneCam = SceneCamera(data.viewportSize);
		data.sceneCam.transform = new Transform(Vector3(0.0f, 0.0f, 1.0f), Vector3::Zero(), Vector3::One());
		data.sceneCam.transform->position.z = 1.0f;
		
		OpenScene("assets/TestProject/Scenes/Default.copper");

		// std::stringstream ss;
		// ss << "Copper Editor - TestProject: " << data.scene.name;
		// data.title = ss.str();

		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);

		Log("Editor Succesfully Initialized");
		Log("--------------------Editor Initialization\n");

	}

	void Run() {

		data.sceneCam.SetCanLook(data.canLookViewport);
		
		if (data.fbo->Width() != data.viewportSize.x || data.fbo->Height() != data.viewportSize.y) {
			
			data.fbo->Resize(data.viewportSize);
			data.sceneCam.Resize(data.viewportSize);
		
		}

		data.fbo->Bind();
		data.scene.Update();
		data.fbo->Unbind();

		data.properties.SetSelectedObject(data.sceneHierarchy.GetSelectedObject());

	}

	void UI() {

		RenderDockspace();
		RenderViewport();
		RenderMenu();
		
		data.sceneHierarchy.UIRender();
		data.properties.UIRender();
		data.fileBrowser.UIRender();

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
		
		uint64_t texture = data.fbo->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(texture), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		data.canLookViewport = ImGui::IsItemHovered();
		
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

	void NewScene() {

		data.scene = Scene();
		data.scene.cam = &data.sceneCam;
		
		data.sceneHierarchy.SetScene(&data.scene);
		data.sceneHierarchy.SetSelectedObject(Object::Null());
		data.properties.SetSelectedObject(Object::Null());
		
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
		data.scene.Deserialize(path);
		data.scene.cam = &data.sceneCam;

		data.changes = false;
		data.title = "Copper Editor - TestProject: ";
		data.title += data.scene.name;
		Input::SetWindowTitle(data.title);

		data.sceneHierarchy.SetScene(&data.scene);
		data.sceneHierarchy.SetSelectedObject(Object::Null());
		data.properties.SetSelectedObject(Object::Null());
		
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


	
}