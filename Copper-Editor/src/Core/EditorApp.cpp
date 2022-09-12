#include "EditorApp.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Object.h"

#include "Engine/Scene/Components/Mesh.h"

#include <ImGui/imgui.h>

#include "ImGui/imgui_internal.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Properties.h"

#include "Viewport/SceneCamera.h"

namespace Editor {
	
	using namespace Copper;

	std::vector<float> vertices{
		//Position				//Color				//Normals
		-0.5f, -0.5f,  0.0f,    1.0f, 0.0f, 0.0f,	0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.0f,    0.0f, 0.0f, 1.0f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 1.0f,	0.0f,  0.0f, -1.0f,

		 // 0.5f, -0.5f,  0.0f,    1.0f, 0.0f, 0.0f,
		 // 0.5f, -0.5f, -1.0f,    0.0f, 1.0f, 0.0f,
		 // 0.5f,  0.5f, -1.0f,    0.0f, 0.0f, 1.0f,
		 // 0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 1.0f,

	};

	std::vector<uint32_t> indices{

		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4

	};

	struct EditorData {

		std::shared_ptr<FrameBuffer> fbo;

		//Scene
		Scene scene;
		//Camera camera;
		
		//Viewport
		UVector2I viewportSize;
		bool isViewportHovered;
		bool isViewportActive;

		//Panels
		SceneHierarchy sceneHierarchy;
		Properties properties;
		
		//Objects
		Object square;
		Object camera;

	};

	EditorData data;

	void Initialize() {

		Log("--------------------Editor Initialization");
		Log("Editor Initialization Started!");

		data.viewportSize = UVector2I(1280, 720);
		data.fbo = std::make_shared<FrameBuffer>(data.viewportSize);
		
		// data.camera = SceneCamera(data.viewportSize);
		// data.camera.transform = new Transform(Vector3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f);

		data.sceneHierarchy = SceneHierarchy();
		data.properties = Properties();

		data.scene = Scene();
		data.sceneHierarchy.SetScene(&data.scene);

		data.square = data.scene.CreateObject("Square");
		data.camera = data.scene.CreateObject("SceneCam");


		Mesh* m = data.square.AddComponent<Mesh>();
		SceneCamera* cam = data.camera.AddComponent<SceneCamera>();
		
		cam->Resize(data.viewportSize);
		data.scene.sceneCam = cam;
		data.camera.transform->position.z = 1.0f;

		m->vertices = vertices;
		m->indices = indices;
		m->Regenerate();

		Log("Editor Succesfully Initialized");
		Log("--------------------Editor Initialization\n");

	}

	void Run() {
		
		data.camera.GetComponent<SceneCamera>()->SetCanLook(data.isViewportActive && data.isViewportHovered);
		
		if (data.fbo->Width() != data.viewportSize.x || data.fbo->Height() != data.viewportSize.y) {
			
			data.fbo->Resize(data.viewportSize);
			data.camera.GetComponent<SceneCamera>()->Resize(data.viewportSize);
		
		}

		data.fbo->Bind();
		data.scene.Update();
		data.fbo->Unbind();

		data.properties.SetSelectedObject(data.sceneHierarchy.GetSelectedObject());

	}

	void UI() {

		RenderDockspace();
		RenderViewport();

		data.sceneHierarchy.UIRender();
		data.properties.UIRender();

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

		data.isViewportHovered = ImGui::IsWindowHovered();
		data.isViewportActive = ImGui::IsWindowFocused();

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		data.viewportSize = UVector2I((uint32_t) windowSize.x, (uint32_t) windowSize.y);

		uint64_t texture = data.fbo->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(texture), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

	}

}