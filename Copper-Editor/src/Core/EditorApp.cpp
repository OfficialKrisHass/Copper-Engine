#include "EditorApp.h"

#include "Panels/SceneHierarchy.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Object.h"

#include "Engine/Scene/Components/Mesh.h"

#include <ImGui/imgui.h>

namespace Editor {

	using namespace Copper;

	std::vector<float> vertices{

		-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 1.0f,

	};

	std::vector<uint32_t> indices{

		0, 1, 2,
		2, 3, 0

	};

	struct EditorData {

		UVector2I viewportSize;
		std::shared_ptr<FrameBuffer> fbo;

		Scene scene;

		SceneHierarchy sceneHierarchy;

		Object square;
		Object camera;

	};

	EditorData data;

	void Initialize() {

		Log("--------------------Editor Initialization");
		Log("Editor Initialization Started!");

		data.viewportSize = UVector2I(1280, 720);
		data.fbo = std::make_shared<FrameBuffer>(data.viewportSize);
		
		data.sceneHierarchy = SceneHierarchy();
		data.scene = Scene();
		data.sceneHierarchy.SetScene(&data.scene);

		data.square = data.scene.CreateObject("Square");
		data.camera = data.scene.CreateObject("Camera");

		Mesh* m = data.square.AddComponent<Mesh>();

		m->vertices = vertices;
		m->indices = indices;
		m->Regenerate();

		Log("Editor Succesfully Initialized");
		Log("--------------------Editor Initialization\n");

	}

	void Run() {

		if (data.fbo->Width() != data.viewportSize.x || data.fbo->Height() != data.viewportSize.y) { data.fbo->Resize(data.viewportSize); }

		data.fbo->Bind();
		data.scene.Update();
		data.fbo->Unbind();

	}

	void UI() {

		RenderDockspace();
		RenderViewport();

		data.sceneHierarchy.UIRender();

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
		data.viewportSize = UVector2I(windowSize.x, windowSize.y);

		uint64_t texture = data.fbo->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(texture), windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

	}

}