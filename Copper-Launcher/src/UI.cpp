#include "UI.h"

#include "Window.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_opengl3.cpp>
#include <imgui_impl_glfw.cpp>

#include <ImGui/imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <yaml-cpp/yaml.h>

#include <iostream>

namespace Launcher::UI {

	void LoadTheme();

	void Initialize() {

		IMGUI_CHECKVERSION();
		ImGui::SetCurrentContext(ImGui::CreateContext());

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NoMouseCursorChange;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/open-sans.regular.ttf", 25.0f);

		LoadTheme();

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) Window::NativePointer(), true);
		ImGui_ImplOpenGL3_Init("#version 460");


	}

	void BeginFrame() {

		// Backend

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

	}
	void Dockspace() {

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("##Dockspace", nullptr, window_flags); //Dockspace

		ImGui::PopStyleVar(2);

	}
	void EndFrame() {

		// Dockspace

		ImGui::End();

		// Backend

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Viewports
		
		ImGuiIO& io = ImGui::GetIO();
		GLFWwindow* backup = glfwGetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup);

	}

	void Shutdown() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();

	}

	void LoadTheme() {

		YAML::Node main;
		try { main = YAML::LoadFile("assets/Launcher.cutheme"); } catch (YAML::ParserException e) {

			std::cout << "Failed to Read Default.cutheme file!\n\t" << e.what() << "\n";
			return;

		}

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = main["Rounding"].as<float>();

		YAML::Node colors = main["Colors"];
		for (YAML::const_iterator it = colors.begin(); it != colors.end(); ++it) {

			uint32 col = it->first.as<uint32>();

			ImVec4 value;
			value.x = it->second[0].as<float>();
			value.y = it->second[1].as<float>();
			value.z = it->second[2].as<float>();
			value.w = it->second[3].as<float>();

			style.Colors[col] = value;

		}

	}

	ImFont* AddFont(const char* path, float size) { return ImGui::GetIO().Fonts->AddFontFromFileTTF(path, size); }
}